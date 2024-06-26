/*
 * letterBox.c
 *
 *  Created on: Apr 3, 2024
 *      Author: frenk
 */

#include "letterBox.h"

char hp_player1[16] = "p1 hp 100";
char hp_player2[16] = "p2 hp 100";
char power_box[16] = "kracht 25";
char angle_box[16] = "hoek r 25";
char turn_player[16] = "     turn p1";


/**
  * @brief Send spi data to write a character to specific spot in a letter box
  * @param uint8_t lettergroup group data
  * @param uint8_t location inside the letter box 
  * @param uint8_t letter that has to be written
  * @returns none
  * 
  * 
  */
void writeLetter(uint8_t groep, uint8_t locatie, uint8_t letter){
	uint16_t totale_data = (groep << 12 | locatie << 8 | letter);
	sendData(WRITE_LETTER,totale_data);
}

/**
  * @brief Converts incoming char array to correct data for spi transfer
  * @param uint8_t	lettergroup group data
  * @param char	text 16 bits in size
  * @returns none
  * 
  * 
  */
void writeBox(uint8_t groep, char tekst[16]){
	char temp_tekst[16];
	uint8_t letter;
	int i,j;
	for(i=0;i<16;i++){
		temp_tekst[i] = tekst[i];
		}

	if(groep == CONT_HP2 || groep == CONT_ANGLE){
		for(i=0;i<9;i++){
			temp_tekst[i+6] = tekst[i];
		}
		for(i=0;i<6;i++){
			temp_tekst[i] = 'A';
		}
	}
	for(i=0;i<15;i++){
		letter = 0;
		for(j = 48;j<58;j++){ //char conversion of numbers to spi data
			if(temp_tekst[i] == 48){
				letter = CHAR_0;
			} else {
				if(temp_tekst[i] == j){
					letter = j-22;
				}
			}
		}
		for(j=97;j<123;j++){ //char conversion of letters to spi data
			if(temp_tekst[i] == j){
				letter = j-96;
			}
		}
		if(temp_tekst[i] == 32){
			letter = 0;
		}
	writeLetter(groep,i,letter);
	}
	writeLetter(groep,15,0);//last character of letterbox has to be empty // minor bug
}
/**
  * @brief Update the Health Points box
  * @param uint8_t player of hp change
  * @param uint8_t new hp number
  * @returns none
  * 
  * 
  */
void updateWriteHP(uint8_t player_write, uint8_t new_hp){
	uint8_t tens = new_hp/10;
	uint8_t ones = new_hp%10;

	if(player_write == 0){
		hp_player1[0] = 'p';
		hp_player1[1] = '1';
		hp_player1[2] = '_';
		hp_player1[3] = 'h';
		hp_player1[4] = 'p';
		hp_player1[5] = '_';
		if(new_hp >= 100){
			hp_player1[6] = '1';
			hp_player1[7] = '0';
			hp_player1[8] = '0';
		} else {

			hp_player1[6] = tens + '0';
			hp_player1[7] = ones + '0';
			hp_player1[8] = '\0';
		}

		writeBox(CONT_HP1,hp_player1);
	} else {
		hp_player2[0] = 'p';
		hp_player2[1] = '2';
		hp_player2[2] = '_';
		hp_player2[3] = 'h';
		hp_player2[4] = 'p';
		hp_player2[5] = '_';
		if(new_hp >= 100){
			hp_player2[6] = '1';
			hp_player2[7] = '0';
			hp_player2[8] = '0';
		} else {

			hp_player2[6] = tens + '0';
			hp_player2[7] = ones + '0';
			hp_player2[8] = '\0';
		}
		writeBox(CONT_HP2,hp_player2);
	}
}
/**
  * @brief Updates numbers in letterboxes of angle and power
  * @param uint16_t current power
  * @param uint16_t current angle
  * @returns none
  * 
  * 
  */
void updateWriteShot(uint16_t current_power, int16_t current_angle){
	int16_t tens = current_power/10;
	int16_t ones = current_power%10;
	uint16_t hoek;
		if(current_power >= 100){
			power_box[7] = '1';
			power_box[8] = '0';
			power_box[9] = '0';
		} else {

			power_box[7] = tens + '0';
			power_box[8] = ones + '0';
			power_box[9] = '\0';
		}

		writeBox(CONT_POWER,power_box);
		if(current_angle < 0){
			hoek = current_angle * -1;
		} else {
			hoek = current_angle;
		}
		tens = hoek/10;
		ones = hoek%10;
		angle_box[0] = 'h';
		angle_box[1] = 'o';
		angle_box[2] = 'e';
		angle_box[3] = 'k';
		angle_box[4] = '_';
		angle_box[6] = '_';
		if(current_angle < 0){
			angle_box[5] = 'l';
			angle_box[7] = tens + '0';
			angle_box[8] = ones +'0';
		} else {
			angle_box[5] = 'r';
			angle_box[7] = tens + '0';
			angle_box[8] = ones + '0';
		}
		writeBox(CONT_ANGLE,angle_box);
}

/**
  * @brief Update letterbox to show the turn
  * @param uint8_t	current player
  * @returns none
  * 
  * 
  */
void updatePlayer(uint8_t current_player){
	if(current_player == 0){
		turn_player[11] = '1';
	} else {
		turn_player[11] = '2';
	}
	writeBox(CONT_REGEL1,turn_player);
}

