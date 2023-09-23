# Terms of use
##### [LICENSE](LICENSE)

# Contacts
[Discord](https://discord.gg/Ec5t3MwnaE)

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


## You can support the author of the mod via PayPal:
[https://paypal.me/terjebruoygard](https://paypal.me/terjebruoygard)

**With love - Terje Bruoygard**
