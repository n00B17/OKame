# OKame

**Status/ Expectation Management:**\
 - I am a noob (hence the name)..
 - ..web-searched most of this (so my ability to explain is limited..)
 - ..and just started messing with github as well.
 - I played with arduino before and really hated Platformio ...initially..
 - ..but -unfortunately- it makes sense and is a lot easier when using it with the VS Code Editor and it's plugins
   

**Description:**\
This is my variation of a minikame quadruped robot thingy.
I basically wanted to have a kame like robot but utilizing an esp32 (or esp8266...later) to be controlled via web interface.\
It seems there once were more of those kind of projects but searching the internet ended in a lot of dead links..

**Functions:**
 - Besides the web controls (see picture below) with some movements it inherited from JavierIH's kame..
 - ..it can be updated via web (OTA, Over The Air)..
 - ..and has a simple non-captive WiFiManager, meaning:
   + by default it tries to connect to a known wifi network (if one was set)
   + if that fails it sets up an own access point named "kame", no password
   + it can be run/ controlled in access point mode
   + on the bottom right is a button to call the wifi-manager page to set and save new credentials and connect to that one (it only stores one set of credentials for now)
   + if connection fails it falls back to access point mode
- in ap as well as sta mode it advertises itself via mDNS (multicast DNS)
  + ..which doesnt work for android by default => I use the ["BonjourBrowser"](https://play.google.com/store/apps/details?id=de.wellenvogel.bonjourbrowser)

**Credits:**\
Go to.. 
 - JavierIH (https://github.com/JavierIH/miniKame) for most of the code as well the stl files (I messed with those as well.. a bit)
 - MusaW for creating this here: https://www.instructables.com/Simple-NodeMCU-18-DOF-Hexapod ... from whom I stole most of the web control parts


**Explanation for the actual magic:**\
See this link: https://oberschweiber.com/2020/03/06/minikame-motion.html



**OKame:**\
..looks like this\
![Nicolas_Bot](https://github.com/n00B17/OKame/assets/46864870/b2c25fa0-bac4-4f04-9b8c-c9d57355643a)




**WebControl:**\
..looks like this\
![KameWebControl_2](https://github.com/n00B17/OKame/assets/46864870/5293d561-6968-4506-a97c-cfb61d3332c4)
