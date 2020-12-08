import subprocess
import sys
import pipes
moves_list_without_end = ["AU1","ED3","FD3","GR2","QR2","PL1","RD1","OR3"] 					# enter moves as strings, for example ["AR1","AR1"], order does not matter
all_perms = list(itertools.permutations(moves_list_without_end))
f = open("all_perm_results.txt", "w+")
for (i,perm) in enumerate(all_perms):
	route ="sheep.exe" 									# enter route to your sheep.exe file inside Popen as string

    process = subprocess.Popen([route], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    perm = list(perm)
	end_move = 	"XU3"								# enter end move here as string
    perm.append(end_move)
    perm_string = ""
    for move in perm:
        perm_string += move + " "
    perm_string = perm_string[:-1]
    process.stdin.write(bytes(perm_string, "ascii"))
    
    str_ret = process.communicate()[0].decode("ascii")
    if (str_ret != ""):
        f.write(("*****************************"))
        f.write("The current permutation is: " + perm_string +"\n")
        f.write("The result is:\n" + str_ret)
        print("*****************************")
        print("The current permutation is: " + perm_string +"\n")
        print("The result is:\n" + str_ret)
    
print("Done running")
f.close()