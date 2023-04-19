import datetime


DUMPEDWITH = "Dumped with HydraDumper | Discord Uniqua#6289"
DUMPDATE = "Dumped at " + datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")

class DumpFormat:
    name : str = "dump"
    
    def __init__(self, dump:dict):
        self.dump = dump
        self.src = ""
        self.start()

    def start(self) -> None:
        self.format()
        self.src = self.head() + self.src

    def head(self) -> str:
        return ""

    def format(self) -> None:
        pass




