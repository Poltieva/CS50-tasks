from nltk import sent_tokenize, ngrams


def lines(a, b):
    """Return lines in both a and b"""

    lines_a = set(line for line in a.split("\n"))
    lines_b = set(line for line in b.split("\n"))

    return lines_a.intersection(lines_b)


def sentences(a, b):
    """Return sentences in both a and b"""

    sents_a = set(sent_tokenize(a))
    sents_b = set(sent_tokenize(b))
    return sents_a.intersection(sents_b)


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""
    ngrams_a = set("".join(gram) for gram in ngrams(a, n))
    ngrams_b = set("".join(gram) for gram in ngrams(b, n))
    return ngrams_a.intersection(ngrams_b)
