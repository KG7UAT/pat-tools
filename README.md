# pat-tools

 Pat-tools is a set of tools I created to help make connecting to and managing my list of gateways easier. Pat-tools lets you create lists with your favorite gateways for quick an easy access and faster connections. Pat-tools also helps taking the guesswork out of which gateway to connect to.

 Pat-tools is a new program I've developed and there is still alot that needs to be done. Its most definitely a work in progress.

### NEW!!
 A new version of pat-tools has been released which has been completely rewritten in C++. No more slow Python!

### Installation

 First pull the software from the repository:

 `git clone https://github.com/KG7UAT/pat-tools.git`

 CD into the directory:

 `cd pat-tools`
 
 Install:
 
 `./install`
 
 To update, run `git pull` and run the install command again.
 
 That's it!
 
 ### Using pat-tools
 
 To create a new list:
 
 `pat-tools new-list [list-name]`
 
 To add a new gateway to the list:
 
 `pat-tools add [list-name]`
 
 To view gateways in your list:
 
 `pat-tools list [list-name]`
 
 If you've got rigctl set up, you can connect to stations in your list by using:
 
 `pat-tools connect [list-name]`
 
 Make sure to replace `[list-name]` with the actual name of your list. So for example, if you created a list named `base`, you would use the command `pat-tools connect base`
 
 Not sure what stations you can connect to? Executing `pat-tools compile-list [list-name] [band]` will make pat attempt to connect to every Winlink station within the band specified. If the connection is successful, it is saved to the list specified. This a rather tedious way to do it, but I haven't been able to find a better way. It works though and its guaranteed to find a station to connect with.

 Pat-tools is still under development and there's a lot I want to do. Some of the code definitely needs to be rewritten as there are better ways, nevertheless it works for now. In the future I want to add a GUI and maybe move pat-tools over to C/C++ for faster, more efficient execution.

73,

KG7UAT
