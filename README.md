### This mod is the property of the DAYZ community, so you can modify and use it on your servers and projects at your discretion. 
### Sale of this mod or mods containing the code of the Syberia project is prohibited!

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
