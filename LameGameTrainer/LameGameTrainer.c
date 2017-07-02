#include <stdio.h>
#include <string.h>
#include <windows.h>

#define MAX_CHEATS 512

/* structure for hot keys */
typedef struct {
	int fsModifiers;
	UINT vkCode;
} HOTKEY;

/* structure for cheats */
typedef struct {
	HOTKEY hotkey;
	char cheat_code[256];
} CHEAT;

/* structure for keys and their respective virtual-key codes */
const static struct {
	char *keyString;
	UINT vkCode;
} specials[] = {
		{"LBUTTON", 0x01},
		{"RBUTTON", 0x02},
		{"CANCEL", 0x03},
		{"MBUTTON", 0x04},
		{"XBUTTON1", 0x05},
		{"XBUTTON2", 0x06},
		{"BACKSPACE", 0x08},
		{"TAB", 0x09},
		{"CLEAR", 0x0c},
		{"ENTER", 0x0d},
		{"PAUSE", 0x13},
		{"CAPSLOCK", 0x14},
		{"ESC", 0x1b},
		{"SPACE", 0x20},
		{"PAGEUP", 0x21},
		{"PAGEDOWN", 0x22},
		{"END", 0x23},
		{"HOME", 0x24},
		{"LEFT", 0x25},
		{"UP", 0x26},
		{"RIGHT", 0x27},
		{"DOWN", 0x28},
		{"SELECT", 0x29},
		{"PRINT", 0x2A},
		{"EXECUTE", 0x2b},
		{"PRINTSCR", 0x2c},
		{"INS", 0x2d},
		{"DEL", 0x2e},
		{"HELP", 0x2f},
		{"LWIN", 0x5b},
		{"RWIN", 0x5c},
		{"NUMPAD0", 0x60},
		{"NUMPAD1", 0x61},
		{"NUMPAD2", 0x62},
		{"NUMPAD3", 0x63},
		{"NUMPAD4", 0x64},
		{"NUMPAD5", 0x65},
		{"NUMPAD6", 0x66},
		{"NUMPAD7", 0x67},
		{"NUMPAD8", 0x68},
		{"NUMPAD9", 0x69},
		{"MULTIPLY", 0x6a},
		{"ADD", 0x6b},
		{"SEPARATOR", 0x6c},
		{"SUBTRACT", 0x6d},
		{"DECIMAL", 0x6e},
		{"DIVIDE", 0x6f},
		{"F1", 0x70},
		{"F2", 0x71},
		{"F3", 0x72},
		{"F4", 0x73},
		{"F5", 0x74},
		{"F6", 0x75},
		{"F7", 0x76},
		{"F8", 0x77},
		{"F9", 0x78},
		{"F10", 0x79},
		{"F11", 0x7a},
		{"F12", 0x7b},
		{"F13", 0x7c},
		{"F14", 0x7d},
		{"F15", 0x7e},
		{"F16", 0x7f},
		{"F17", 0x80},
		{"F18", 0x81},
		{"F19", 0x82},
		{"F20", 0x83},
		{"F21", 0x84},
		{"F22", 0x85},
		{"F23", 0x86},
		{"F24", 0x87},
		{"NUMLOCK", 0x90},
		{"SCROLLLOCK", 0x91},
		{"LSHIFT", 0xa0},
		{"RSHIFT", 0xa1},
		{"LCTRL", 0xa2},
		{"RCTRL", 0xa3},
		{"ALT", 0x12}
};

/* SendKeys() uses keybd_event to send a whole word */
void SendKeys(char *word) {
	int i = 0;
	while (word[i] != 0) {
		UINT vKeyCode = VkKeyScan(word[i]);
		UINT vScanCode = MapVirtualKey(vKeyCode, 0);
		keybd_event(vKeyCode, vScanCode, 0, 0);
		keybd_event(vKeyCode, vScanCode, KEYEVENTF_KEYUP, 0);
		i++;
	}
}

/* Parsing hot keys (looks shitty but does work) */
int parse_hotkey(char *hotkey, int stage, HOTKEY *result) {
	if (hotkey[0] == 0x7b) {
		int fsModifiers = 0;
		char hotkey_tmp[128], *prev, *next;
		
		strncpy(hotkey_tmp, hotkey, 128);
		hotkey_tmp[strlen(hotkey_tmp) - 1] = 0;
		
		/* storing what's between {} and what's after them*/
		if ((prev = strtok(&hotkey_tmp[1], "}")) == NULL)
			return FALSE;
		if ((next = strchr(hotkey, 0x7d)) == NULL)
			return FALSE;
		
		/* starting stage */
		if (stage == 0) {
			/* if there is nothing "after", then prev must be a usual "special" key, check if it's in the list and store in result */
			if (next[1] == '\0') {
				int i;
				for (i = 0; i < 78; i++) {
					if (stricmp(prev, specials[i].keyString) == 0) {
						if (result != 0) {
							(*result).fsModifiers = 0;
							(*result).vkCode = specials[i].vkCode;
						}
						return TRUE;
					}
				}
				return FALSE;
			}
			/* if there is something "after", then prev must be a "hold" key before the real key, compare against the hold keys */
			if (stricmp(prev, "ALT") == 0)
				fsModifiers = 1;
			else if (stricmp(prev, "CTRL") == 0)
				fsModifiers = 2;
			else if (stricmp(prev, "SHIFT") == 0)
				fsModifiers = 4;
			else if (stricmp(prev, "WIN") == 0)
				fsModifiers = 8;
			else
				return FALSE;
			
			/* going to stage 2, parsing the "after" */
			return parse_hotkey(&next[1], fsModifiers, result);
		} else {
			int i;
			/* there cannot be something after, only one key (special in this case) is allowed after the "hold key" */
			if (next[1] != '\0')
				return FALSE;
			/* comparing against the list */
			for (i = 0; i < 78; i++) {
				if (stricmp(prev, specials[i].keyString) == 0) {
					if (result != 0) {
						(*result).fsModifiers = stage;
						(*result).vkCode = specials[i].vkCode;
					}
					return TRUE;
				}
			}
			return FALSE;
		}
	} else {
		/* can be stage 1 or stage 2, must be one character that is alphanumeric. if in stage 1, then there are no "hold keys" 
		(and fsModifiers will be 0), if not then fsModifiers in the struct will hold the value of the fsModifiers */
		if (strlen(hotkey) == 1) {
			if (isalnum(hotkey[0])) {
				if (result != 0) {
					(*result).fsModifiers = stage;
					(*result).vkCode = VkKeyScan(hotkey[0]);
				}
				return TRUE;
			}
			return FALSE;
		}
		return FALSE;
	}
}

/* Cheatcode must be only alphanumeric */
int parse_cheatcode(char *hotkey) {
	int i = 0;
	while (hotkey[i] != 0) {
		if (!isalnum(hotkey[i]))
			return FALSE;
		i++;
	}
	return TRUE;
}

/* Writing hot keys and cheat codes in the default configuration file */
void write_config(char *hotkey, char *cheat_code) {
	FILE *config;
	char cheat_string[290] = "";
	
	/* the delimeter is | */
	snprintf(cheat_string, 290, "%s|%s\n", hotkey, cheat_code);
	
	if ((config = fopen("LAME_config.conf", "a")) == NULL) {
		printf("Error opening the config file, check the directory/file has proper permissions.\r\n");
		exit(0);
	}
	
	if(fputs(cheat_string, config) == EOF) {
		printf("Error writing to config\r\n");
		exit(0);
	}
	printf("Wrote the cheat successfully\r\n");
	fclose(config);
}

/* Loading configuration files, registering the hot keys and returning CHEAT structures with the cheats in the config file */ 
CHEAT *load_config(char *config_file) {
	/* only MAX_CHEATS allowed, calloc zeroes by itself, no need to further initialization */
	CHEAT *cheats = calloc(MAX_CHEATS, sizeof(CHEAT));
	FILE *config;
	char cheat_string[290];
	int i = 0;
	
	if ((config = fopen(config_file, "r")) == NULL) {
		printf("The configuration file you provided does not exist.\r\n");
		exit(0);
	}
	/* Reading line-by-line and preventing OOB writes */
	while (fgets(cheat_string, 290, config) && i < MAX_CHEATS) {
		HOTKEY hot_key = {0};
		char *hotkey, *cheat_code;
		char cheat_string_tmp[290];
		
		snprintf(cheat_string_tmp, 290, "%s", cheat_string);
		
		/* making sure the line is in the configuration format, and storing the hot key and respective cheat code */
		if ((hotkey = strtok(cheat_string_tmp, "|")) == NULL) {
			printf("Make sure you entered a vaild config file.\r\n");
			exit(0);
		}
		if ((cheat_code = strchr(cheat_string, 0x7c)) == NULL) {
			printf("Make sure you entered a vaild config file.\r\n");
			exit(0);
		}
		cheat_code = &cheat_code[1]; /* removing the | */
		cheat_code[strcspn(cheat_code, "\n")] = 0; /* removing the new line */
		
		/* parsing the hot key and storing in hot_key, and making sure that cheat code is alphanumeric */
		if (!parse_hotkey(hotkey, 0, &hot_key) || !parse_cheatcode(cheat_code)) {
			printf("Invalid strings in configuration file.\r\n");
			exit(0);
		}
		
		/* Registering the hot keys with their respective fsModifiers  */
		if (RegisterHotKey(NULL, 1, hot_key.fsModifiers, hot_key.vkCode)) {
			printf("Registired hotkey %s!\r\n", hotkey);
			/* storing hotkey information and their respective cheat_codes to be used later in identifying WM_HOTKEY and sending the respective cheat code to each HOTKEY */
			cheats[i].hotkey.fsModifiers = hot_key.fsModifiers;
			cheats[i].hotkey.vkCode = hot_key.vkCode;
			snprintf(cheats[i].cheat_code, 256, "%s", cheat_code);
		}
		i++;
	}
	fclose(config);
	return cheats;
}

/* Message loop waiting for WM_HOTKEY messages, checking for the respective cheat codes to the hot keys 
(fsModifiers is the low-order word of lParam, and the vkCode of the key is the high-order word of lParam) to send them using SendKeys */ 
void wait_for_hotkeys(CHEAT *cheats) {
	MSG msg = {0};
	while (GetMessage(&msg, NULL, 0, 0) != 0) {
		if (msg.message == WM_HOTKEY) {
			int i;
			for (i = 0; i < MAX_CHEATS; i++) {
				if (LOWORD(msg.lParam) == cheats[i].hotkey.fsModifiers && HIWORD(msg.lParam) == cheats[i].hotkey.vkCode) {
					printf("Received hotkey\r\n");
					printf("%s\r\n", cheats[i].cheat_code);
					SendKeys(cheats[i].cheat_code);
				}
			}
		}
	}
}

int main(int argc, char *argv[]) {
	CHEAT *cheats; /* cheats loaded for this session */
	FILE *config;
	
	if (argc > 1) {
		cheats = load_config(argv[1]); /* loading the configuration from user-input */
		wait_for_hotkeys(cheats);
		free(cheats); /* freeing to avoid memory leaks */
	}
	else {
		if ((config = fopen("LAME_config.conf", "r")) != NULL) {
			fclose(config); /* there is no need of it, close to allow the deletion of the file */
			
			int message = MessageBoxEx(NULL, "The default config already exists\nPress Yes if you want to use it\nNo if you want to delete it and generate a new one.", "Notification", MB_YESNO | MB_ICONWARNING, 0);
			
			if (message == IDYES) {
				cheats = load_config("LAME_config.conf"); /* loading the default config */
				wait_for_hotkeys(cheats);
				free(cheats); /* freeing to avoid memory leaks */
			} else {
				DeleteFile("LAME_config.conf");
				MessageBoxEx(NULL, "Run me again to generate a new config", "Notification", MB_ICONINFORMATION, 0);
			}
		} else {
			while (TRUE) {
				/* {SHIFT}{SCROLLOCK} is supposed to be the longest hot key, and there is no obvious reason why I chose 256 for cheat codes */
				char hotkey[25], cheat_code[256];
				printf("\r\nHotkey (endpls to stop): ");
	
				if(fgets(hotkey, 25, stdin) == NULL) {
					printf("Error reading hotkey");
					exit(0);
				}
				hotkey[strcspn(hotkey, "\r\n")] = 0;
		
				if (stricmp(hotkey, "endpls") == 0)
					break;
		
				if (!parse_hotkey(hotkey, 0, 0)) {
					printf("Invalid hotkey..\r\n");
					continue;
				}
				printf("\r\nCheat code (endpls to stop): ");
		
				if(fgets(cheat_code, 256, stdin) == NULL) {
					printf("Error reading cheat code");
					exit(0);
				}
				cheat_code[strcspn(cheat_code, "\r\n")] = 0;
		
				if (stricmp(cheat_code, "endpls") == 0)
					break;
		
				if (!parse_cheatcode(cheat_code)) {
					printf("Invalid cheat code..\r\n");
					continue;
				}
				write_config(hotkey, cheat_code);
			}
			MessageBoxEx(NULL, "Thanks for using us!\nRun me again and you can use the cheats!", "Notification", MB_ICONINFORMATION, 0);
		}
	}
}
