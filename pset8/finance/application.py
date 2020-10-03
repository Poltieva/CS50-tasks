import os
from datetime import datetime

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    info = db.execute("SELECT symbol, SUM(number) FROM bought WHERE user_id = :user_id GROUP BY symbol;", user_id=session["user_id"])
    cash = db.execute("SELECT cash from users WHERE id = :user_id;", user_id=session["user_id"])[0]["cash"]
    if info:
        for el in info:
            el["price"] = lookup(el['symbol'])["price"]
            el["total"] = el['SUM(number)'] * el["price"]
        return render_template('index.html', info=info, cash=cash)
    else:
        return render_template('index.html', info=None, cash=cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        num = request.form.get("shares")
        if not symbol:
            return apology("You must provide a symbol", 400)
        elif not num:
            return apology("You must provide a number of shares", 400)
        else:
            try:
                num = int(num)
            except ValueError:
                return apology("You must provide a number of shares", 400)
            quote = lookup(symbol)
            if num < 1:
                return apology("Invalid number of shares", 400)
            elif not quote:
                return apology("Invalid share", 400)
            else:
                price = float(quote["price"])
                symbol = quote["symbol"]
                money = db.execute("SELECT cash FROM users WHERE id=:user_id;", user_id=session["user_id"])[0]["cash"]
                cost = price * num
                if money < cost:
                    return apology("You don't have money for that", 400)
                else:
                    db.execute("INSERT INTO bought ('user_id', 'symbol', 'number', 'price_per_one', 'transacted') \
                                VALUES (:user_id, :symbol, :num, :price, :date);",
                                user_id=session["user_id"], symbol=symbol, num=num, price=price, date=datetime.now())
                    db.execute("UPDATE users SET cash=:cash WHERE id=:user_id;", cash=money-cost, user_id=session["user_id"])
                    return redirect("/")
    else:
        return render_template("buy.html")



@app.route("/check", methods=["GET"])
def check():
    """Return true if username available, else false, in JSON format"""
    name = request.args.get("username")
    result = db.execute("SELECT id FROM users WHERE username = :name;", name=name)
    if result:
        return jsonify(False)
    else:
        return jsonify(True)


@app.route("/settings", methods=["GET", "POST"])
@login_required
def settings():
    if request.method == "POST":
        new_pwd = request.form.get("new_password")
        db.execute("Update users SET hash = :pwd WHERE id = :id", pwd=generate_password_hash(new_pwd), id=session["user_id"])
        return redirect("/")
    else:
        username = db.execute("SELECT username FROM users WHERE id = :id;", id=session["user_id"])[0]["username"]
        return render_template("settings.html", username=username)


@app.route("/history")
@login_required
def history():
    actions = db.execute("SELECT symbol, number, price_per_one*number, transacted FROM bought WHERE user_id = :user_id;",
                            user_id=session["user_id"])
    return render_template("history.html", actions=actions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("/quote.html", quotes=None)
    else:
        symbol = request.form.get("symbol")
        quotes = lookup(symbol)
        if not symbol or not quotes:
            return apology("you must provide a valid symbol", 400)
        return render_template("/quote.html", quotes=quotes)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Ensure username was submitted
        username = request.form.get("username")
        if not username:
            return apology("must provide username", 400)

        # Ensure password was submitted
        password = request.form.get("password")
        confirm = request.form.get("confirmation")
        if not password:
            return apology("must provide password", 400)
        if not confirm:
            return apology("you must confirm your password", 400)

        elif password != confirm:
            return apology("you must confirm your password", 400)

        # Query database for username
        rows = db.execute("SELECT id FROM users WHERE username = :username",
                          username=username)
        if rows:
            return apology("this username is already taken", 400)


        try:
            db.execute("INSERT INTO users ('username', 'hash') VALUES(:username, :hash_value);",
                          username=username, hash_value=generate_password_hash(password))
            rows = db.execute("SELECT id FROM users WHERE username = :username",
                          username=username)

            # Remember which user has logged in
            session["user_id"] = rows[0]["id"]
            return redirect("/")
        except Exception:
            return apology(Exception, 403)

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        num = int(request.form.get("shares"))
        shares_in_hand = db.execute("SELECT SUM(number) FROM bought WHERE user_id = :user AND symbol = :symbol;",
                                    user=session["user_id"], symbol=symbol)[0]['SUM(number)']

        if shares_in_hand:
            if num > shares_in_hand:
                return apology("You don't have that many shares of this company")
            else:
                quote = lookup(symbol)
                price = float(quote["price"])
                db.execute("INSERT INTO bought ('user_id', 'symbol', 'number', 'price_per_one', 'transacted') \
                    VALUES (:user_id, :symbol, :num, :price, :date);",
                    user_id=session["user_id"], symbol=symbol, num=-num, price=price, date=datetime.now())
                money = db.execute("SELECT cash FROM users WHERE id=:user_id;", user_id=session["user_id"])[0]["cash"]
                db.execute("UPDATE users SET cash=:cash WHERE id=:user_id;", cash=money-price*num, user_id=session["user_id"])
                return redirect("/")
        else:
            return apology("You don't have shares of this company")

    else:
        return render_template("sell.html",
        symbols=db.execute("SELECT DISTINCT symbol FROM bought WHERE number > 0 AND user_id=:user_id;",
        user_id=session["user_id"]))


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
