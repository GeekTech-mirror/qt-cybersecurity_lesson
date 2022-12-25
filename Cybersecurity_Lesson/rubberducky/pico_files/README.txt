TO SET UP THE PYCO (RASPBERRY PI RUBBER DUCKY-PYTHON BASED)

1. HOLD THE BOOTSEL button while pluggin in the PICO
	1.b) USE A JUMPER TO CONNECT PINS 1 & 3 TO AVOID AUTO RUNNING THE DUCKY AT THE END PRIOR TO DRAGGING THE .uf2 file

2. Drag the .uf2 file into the PICO's root directory
	2.b) Wait for it to restart
	2.c) (Optional) IF there are already files present, delete them for a fresh start

3. Drag the contents of the LIB folder into the PYCO's LIB directory

4. Drag boot.py over

5. Delete the code.py on the PYCO, drag this folder's code.py onto the PICO

6. Move payload-img.png to the PYCO

7. Move payload.dd to the PYCO
	*NOTE IT WILL AUTO RUN UNLESS A JUMPER CONNECTS PINS 1 & 3 AS STATED IN STEP 1.b