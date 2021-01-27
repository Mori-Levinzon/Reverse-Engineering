
from attack_shell import *


if __name__ == "__main__":
	proc = subprocess.Popen('hw4_client.exe',stdin=subprocess.PIPE )

	attack(proc)

	solve= r"echo 'Fires: False\nRivals: True\nKnights Infected: False\nRobber Hunted: True' > config\attack.config \n"
	input_t = byteArrNumber(solve,7)
	for j in input_t:
		proc.stdin.write(j)
		proc.stdin.flush()
	


	proc.kill()