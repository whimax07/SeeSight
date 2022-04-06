// By Max Whitehouse, 02/04/2022.
// A quick check of risk odds and strats.
// If a force of between 1 and 'max_attackers' attacks a sequence of cells with defenders and print the probably 
// calculated by brute force.
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



const int NUMBER_OF_RUNS = 1000000;



int roll_dice() {
    // Note(Max): I think 6 is small engought that I don't care about the mod bias.
    return (rand() % 6) + 1;
}


void pop_rolls(int rolls[], int forces, int number_of_dice) {
    for (int i = 0; i < number_of_dice; i ++) {
        if (forces <= 0) {
            break;
        }
        rolls[i] = roll_dice();
        forces --;
    }

    // This sorts a len 3 array.
    if (rolls[0] < rolls[1]) {
        int temp = rolls[1];
        rolls[1] = rolls[0];
        rolls[0] = temp;
    }

    if (rolls[1] < rolls[2]) {
        int temp = rolls[2];
        rolls[2] = rolls[1];
        rolls[1] = temp;
    }

    if (rolls[0] < rolls[1]) {
        int temp = rolls[1];
        rolls[1] = rolls[0];
        rolls[0] = temp;
    }
}


void resolve_rolls(int ack_rolls[], int *attackers, int def_rolls[], int *defender) {
    int count = 0;
    while (count < 2 && *attackers > 0 && *defender > 0) {
        if (def_rolls[count] >= ack_rolls[count]) {
            (*attackers) --;
        } else {
            (*defender) --;
        }
        count ++;
    }
}


int resolve_battle(int attacker, int defender) {
    int def_rolls[3] = {0, 0, 0};
    int ack_rolls[3] = {0, 0, 0};

    while (defender > 0) {
        if (attacker == 0) {
            return 0;
        }

        pop_rolls(def_rolls, defender, 2);
        pop_rolls(ack_rolls, attacker, 3);
        
        resolve_rolls(ack_rolls, &attacker, def_rolls, &defender);
    }

    return attacker;
}


void single_run(int attacker, const int defenders[], size_t number_of_stages, int attackers_left[], int round_completed[]) {  
    for (int i = 0; i < number_of_stages; i ++) {
        int defender = defenders[i];
        attacker = resolve_battle(attacker, defender);

        if (attacker <= 0) {
            return;
        }
        
        attackers_left[i] += attacker;
        round_completed[i] ++;
    }
}


void print_setup(int defenders[], int len_defenders) {
    printf("Running defenders {");
    for (int i = 0; i < len_defenders - 1; i ++) {
        printf("%d, ", defenders[i]);
    }
    printf("%d}. \n", defenders[len_defenders - 1]);
}


void print_results(int attacker, int len_defenders, int round_completed[], int attacker_left[]) {
    printf("Attackers = %d. \n", attacker);

    printf("Round reached odds [Average Attacker Left]: ");

    for (int i = 0; i < len_defenders - 1; i ++) {
        int per_round = round_completed[i] * 100 / NUMBER_OF_RUNS;
        int avg_left = round(attacker_left[i] * 10 / (double) NUMBER_OF_RUNS);

        printf("%d) %d\% [%d.%d], ", i, per_round, avg_left / 10, avg_left % 10);
    }

    int per_round = round_completed[len_defenders - 1] * 100 / NUMBER_OF_RUNS;
    int avg_left = round(attacker_left[len_defenders - 1] * 10 / (double) NUMBER_OF_RUNS);
    printf("%d) %d\% [%d.%d]. \n", len_defenders - 1, per_round, avg_left / 10, avg_left % 10);
}


void run_situation(int max_attackers, int defenders[], int len_defenders) {
    print_setup(defenders, len_defenders);

    for (int attacker = 1; attacker <= max_attackers; attacker ++) {
        int attacker_left[] = {0, 0};
        int round_completed[] = {0, 0};

        for (int i = 0; i < NUMBER_OF_RUNS; i ++) {
            single_run(attacker, defenders, len_defenders, attacker_left, round_completed);
        }

        print_results(attacker, len_defenders, round_completed, attacker_left);
    }
}


void run() {
    int defenders1[] = {3, 3};
    run_situation(15, defenders1, 2);

    int defenders2[] = {5, 1};
    run_situation(15, defenders2, 2);
}



int main(int argc, char *argv[]) {
    // Init rand.
    srand(time(NULL));

    run();
}
