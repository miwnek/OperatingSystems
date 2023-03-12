#include <stdio.h>
#include "library.h"

int main() {
    Extended_array ex = initialize_extended_array();

    char text[] = "lol.txt";
    char texty[] = "lmao";
    printf("Executed with result: %d\n",execute_wc(&ex, text));
    Wc_result* received_one = get_block_on_index(&ex, 0); 
    printf("Received - l: %d, w: %d, c: %d\n", received_one->lines, received_one->words, received_one->chars);

    printf("Executed with result: %d\n",execute_wc(&ex, texty));
    Wc_result* received_two = get_block_on_index(&ex, 1); 
    printf("Received - l: %d, w: %d, c: %d\n", received_two->lines, received_two->words, received_two->chars);

    printf("Editing went right!\n");
    printf("Or did it?");

    return 0;
}
