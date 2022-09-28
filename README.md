### Terms of use and copyright (Apache 2.0 LICENSE)
##### - Sale of this mod or mods containing the code of the Syberia project is prohibited!
##### - You are obliged to indicate the authors of the mod if you use the Syberia mod or parts of it in your projects/servers.
##### - All modified versions of the Syberia mod must be publicly available on github and be forks of the original mod.
##### - It is allowed to repack and modify the mod only with the indication of the authorship of the original mod present on the steam workshop page and in the game as watermark.
##### - Monetization of dayz servers using this mod is allowed.

# How to build server PBO:
1. Create SyberiaServer folder on your P: drive.
2. Copy content from "src" folder to "P:/SyberiaServer" folder.
3. Use MikeroTools or AddonBuilder to build PBO.

# How to build Database app for server:
1. Open "SyberiaAdditionalSoftware.sln" in Visual Studio 2019
2. Change solution configuration to "Release"
3. Right click on solution in files explorer and press "Restore Nuget Packages"
4. Build the "SyberiaServerManager" project. (Right click on SyberiaServerManager project in files explorer and press "Rebuild")
5. Copy all files from "SyberiaServer/manager/bin/Release" folder to your folder (any empty folder).

# How to start the server:
1. Copy "SyberiaServer.pbo" to Addons folder in root of dayz server.
2. Copy the "DatabaseOptions.json" file to "DAYZ_SERVER_FOLDER/profiles" folder. (change port number for database server if needed. Every server instance should have unique database port)
3. Change login and password from webpanel in "credentials.cfg" inside Database folder.
4. Start the "SyberiaServerManager.exe --serverdir "PATH TO DAYZ SERVER ROOT DIR" --port 4307" from cmd as administrator (from database folder).
5. Starts the DayZ Server with SyberiaScripts mod. (See SyberiaScripts repository for get client-side mod sources)


## If you want to support me:
```
BTC: bc1q395yw8as5v9mn4yzls5y78av0q342e2cwdrhdh 
```

**With love - Terje Bruoygard**
