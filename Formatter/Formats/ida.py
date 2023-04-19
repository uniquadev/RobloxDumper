from .dump import DumpFormat, DUMPEDWITH, DUMPDATE


class IdaFormat(DumpFormat):
    name = "ida.py"
    
    def head(self):
        return (
            f"# For Roblox WinClient {self.dump['version']}\n"
            f"# {DUMPEDWITH}\n"
            f"# {DUMPDATE}\n"
       )
    
    def rename(self, subs:dict):
        if not subs:
            return
    
        for name, info in subs.items():
            offset = info["address"]
            self.src += f"idc.set_name(base+{offset}, '{name}')\n"
        

    def format(self):
        self.src = "base = ida_nalt.get_imagebase()\n\n"
        subs = self.dump["subs"]
    
        self.rename(subs)

        self.src += "\nprint('-- SCRIPT LOADED, HAVE FUN --')"