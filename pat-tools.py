#!/usr/bin/python3

import sys
import os
import subprocess

if sys.argv[1] == "list-all":
	if len(sys.argv) == 2:
		os.system("pat rmslist -m ardop")
	elif len(sys.argv) == 3:
		os.system("pat rmslist -m ardop | grep " + sys.argv[2])
elif sys.argv[1] == "new-list":
	f = open("/usr/local/bin/lists/" + sys.argv[2] + ".list", "w")
#	f.write("")
	print("Created new list: " + str(sys.argv[2]))
elif sys.argv[1] == "add":
	callsign = input("Enter Callsign: ")
	freq = input("Enter dial frequency in kHz: ")

	print("\nAdded new entry to " + str(sys.argv[2]))

	f = open("/usr/local/bin/lists/" + str(sys.argv[2]) + ".list", "a")
	f.write(callsign + "," + freq + "\n")
	f.close()
elif sys.argv[1] == "remove":
	f = open("/usr/local/bin/lists/" + str(sys.argv[2]) + ".list", "r")
	stations = str(f.read()).split("\n")
	stations.remove("")

	for s in stations:
		print(s[:s.rfind(",")])

	call = input("Type call of station to delete: ")

	for s in stations:
		if s.startswith(call) == True:
			stations.remove(s)

	f = open("/usr/local/bin/lists/" + str(sys.argv[2]) + ".list", "w")
	f.write("")
	f.close()

	for s in stations:
		f = open("/usr/local/bin/lists/" + str(sys.argv[2]) + ".list", "a")
		f.write(s + "\n")
		f.close()

	print("\nRemoved entry")
elif sys.argv[1] == "delete-list":
	question = input("Are you sure you want to delete this list? [y/n] ")

	if question == "y":
		os.remove("/usr/local/bin/lists/" + str(sys.argv[2]) + ".list")
		print("Deleted list: " + str(sys.argv[2]))
	elif question == "n":
		print("Aborted")
	else:
		print("Aborted")
elif sys.argv[1] == "connect":
	bands = [["1", "160m"], ["3", "80m"], ["5", "60m"], ["7", "40m"], ["10", "30m"], ["14", "20m"], ["18", "17m"], ["21", "15m"], ["24", "12m"], ["28", "10m"], ["50", "6m"], ["144", "2m"]]
	frequency = "0"

	if len(sys.argv) == 4:
		for b in bands:
			if sys.argv[3] == b[1]:
				frequency = b[0]

	f = open("/usr/local/bin/lists/" + str(sys.argv[2]) + ".list", "r")
	stations = str(f.read()).split("\n")
	stations.remove("")

	for s in stations:
		if len(sys.argv) == 4:
			if frequency == str(float(s.split(",")[1]) / 1000).split(".")[0]:
				os.system("pat connect ardop:///" + s.replace(",", "?freq="))
				print("\n")
		else:
			os.system("pat connect ardop:///" + s.replace(",", "?freq="))
			print("\n")
elif sys.argv[1] == "list":
	bands = [["1", "160m"], ["3", "80m"], ["5", "60m"], ["7", "40m"], ["10", "30m"], ["14", "20m"], ["18", "17m"], ["21", "15m"], ["24", "12m"], ["28", "10m"], ["50", "6m"], ["144", "2m"]]
	frequency = "0"

	if len(sys.argv) == 4:
		for b in bands:
			if sys.argv[3] == b[1]:
				frequency = b[0]

	if len(sys.argv) == 2:
		for l in os.listdir("/usr/local/bin/lists/"):
			print(l.replace(".list", " "))
	elif len(sys.argv) >= 3:
		f = open("/usr/local/bin/lists/" + str(sys.argv[2]) + ".list", "r")
		stations = str(f.read()).split("\n")
		stations.remove("")

		for s in stations:
			if len(sys.argv) == 4:
				if frequency == str(float(s.split(",")[1]) / 1000).split(".")[0]:
					os.system("pat rmslist -m ardop | grep " + s.replace(",", "?freq="))
			else:
				os.system("pat rmslist -m ardop | grep " + s.replace(",", "?freq="))
elif sys.argv[1] == "compile-list":
	num = 0
	prefix = ['A', 'K', 'N', 'W']
	list = []
	band = "none"
	frequency = "0"
	bands = [["1", "160m"], ["3", "80m"], ["5", "60m"], ["7", "40m"], ["10", "30m"], ["14", "20m"], ["18", "17m"], ["21", "15m"], ["24", "12m"], ["28", "10m"], ["50", "6m"], ["144", "2m"]]

	if len(sys.argv) == 4:
		band = sys.argv[3]

	if band != "none":
		for b in bands:
			if b[1] == band:
				frequency = b[0]

	for p in prefix:
		output = subprocess.run(['pat', 'rmslist', '-s', '-m', 'ardop', p], stdout=subprocess.PIPE).stdout.decode('utf-8').split("\n")
		output.pop(0)
		output.pop(len(output) - 1)

		for o in output:
			if o != "":
				if o.split(" ")[len(o.split(" ")) - 3].split(".")[0] == frequency or band == "none":
					list.append(o.split(" ")[len(o.split(" ")) - 1])


	print("Pat will attempt to connect to " + str(len(list)) + " stations. This could take about " + str(round(len(list) * .75)) + " minutes.\n\n")

	for l in list:
		num += 1
		print(str(num) + ". " + l)
		output = subprocess.run(['pat', 'connect', l], stdout=subprocess.PIPE).stdout.decode('utf-8')

		if "Unable to establish connection to remote" in output:
			print(" | connection failed\n")
		elif "Connected to" in output:
			f = open("/usr/local/bin/lists/" + str(sys.argv[2]) + ".list", "a")
			f.write(l[9:].replace("?freq=",",") + "\n")
			f.close()

			print(" | connection successful\n")
		else:
			print(" | uh oh...something happended")
elif sys.argv[1] == "version":
	print("v0.1")
else:
	print("Command " + sys.argv[1] + " not understood.")
