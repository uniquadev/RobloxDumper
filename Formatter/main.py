# This file is part of the uniquadev/RobloxDumper and is licensed under MIT License; see LICENSE.txt for details
# Run this script to create custom dump formats from the dump.json

# imports
import json
from os import makedirs, path
from os.path import isdir
from shutil import rmtree

# formats
from Formats.ida import IdaFormat
from Formats.dump import DumpFormat

# locate folders
DIR = path.join(path.dirname(path.abspath(__file__)), "../build/Release")
TEMP = path.join(DIR, "temp")

DUMP = json.load(
    open(f"{DIR}/dump.json", "r")
)

VERSION = DUMP["version"]

FORMATTERS : list[DumpFormat] = [
    IdaFormat
]

def load_formats() -> None:
    # delete temp
    if isdir(TEMP):
        rmtree(TEMP)
    # make temp folder
    makedirs(TEMP)
    # loop all formats
    for formatter in FORMATTERS:
        f = formatter(DUMP)
        with open(f"{TEMP}/{VERSION}-{formatter.name}", "w+") as o:
            o.write(f.src)


def upload():
    pass

def main():
    load_formats()
    upload()


if __name__ == "__main__":
    main()