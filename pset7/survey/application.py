
import cs50
import csv

from flask import Flask, jsonify, redirect, render_template, request

# Configure application
app = Flask(__name__)

# Reload templates when they are changed
app.config["TEMPLATES_AUTO_RELOAD"] = True


@app.after_request
def after_request(response):
    """Disable caching"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET"])
def get_index():
    return redirect("/form")


@app.route("/form", methods=["GET"])
def get_form():
    return render_template("form.html")


@app.route("/form", methods=["POST"])
def post_form():
    fname = request.form.get("fname")
    lname = request.form.get("lname")
    gender = request.form.get("gender")
    if not fname:
        return render_template("error.html", message="You didn't provide your first name")
    elif not lname:
        return render_template("error.html", message="You didn't provide your last name")
    elif not gender:
        return render_template("error.html", message="You didn't provide your gender")
    else:
        #try:
        animals = []
        for i in range(1, 5):
            animal = request.form.get("animal" + str(i))
            if animal:
                animals.append(animal)
        if not animals:
            animals.append("no animals")
        status = request.form.get("status")
        with open("survey.csv", "a") as file:
            writer = csv.writer(file)
            writer.writerow((fname, lname, gender, " ".join(animals), status))
        return redirect("/sheet")
        #except:
            #return render_template("error.html", message="Something went wrong")


@app.route("/sheet", methods=["GET"])
def get_sheet():
    #try:
    with open("survey.csv", "r") as file:
        reader = csv.reader(file)
        people = list(reader)
        return render_template("sheet.html", people=people)
    #except:
     #   return render_template("error.html", message="Something went wrong")
