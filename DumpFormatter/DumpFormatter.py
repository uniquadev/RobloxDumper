# imports
import json
from os import makedirs
from os.path import isdir
from shutil import rmtree
from pickle import TRUE
# formats
from Formats.ida import IdaFormat
from Formats.dump import DumpFormat


DUMP = json.load(open("../out.json", "r"))
VERSION = DUMP["version"]

FORMATTERS : list[DumpFormat] = [
    IdaFormat
]

def load_formats() -> None:
    # delete temp
    if isdir("temp"):
        rmtree("temp")
    # make temp folder
    makedirs("temp")
    # loop all formats
    for formatter in FORMATTERS:
        f = formatter(DUMP)
        with open(f"temp/{VERSION}-{formatter.name}", "w+") as o:
            o.write(f.src)


def upload():
    pass

def main():
    load_formats()
    upload()


if __name__ == "__main__":
    main()