#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>

void set_random_seed();
int randn(int n);
void first_move(std::vector<int> &attempt, int len, int num);

struct guess{
  std::vector<int> attempted_sequence;
  int b_hits;
  int w_hits;
};


bool calculate_fitness(std::vector<int> possible_soln, std::vector<guess> previous_guesses);


struct mm_code_maker{

    void init(int i_length, int i_num){
        length = i_length;
        num = i_num;
    }

    void generate_sequence(){
      for(int i = 0; i < length; i++){
          sequence.push_back(randn(num));
      }
      //sequence = {1, 1, 0};
    }

    void give_feedback(const std::vector<int>& attempt, int& black_hits, int& white_hits){
        black_hits = 0;
        white_hits = 0;

        std::vector<int> sequence_copy;
        for(int a = 0; a < sequence.size(); a++){
            sequence_copy.push_back(sequence[a]);
        }

        for(int i = 0; i < sequence.size(); i++) {
          if(attempt[i] == sequence[i]) {
            black_hits++;
          }

          bool remove = false;
          int remove_index;
          for(int n = 0; n < sequence_copy.size(); n++) {
            if(attempt[i] == sequence_copy[n]) {
              white_hits++;
              remove = true;
              remove_index = n;
              break;
            }
          }
          if(remove) {
            sequence_copy.erase(sequence_copy.begin()+remove_index);
          }
        }

        white_hits = white_hits - black_hits;
    }

    std::vector<int> sequence;

    int length;
    int num;
};

struct mm_solver{

    void init(int i_length, int i_num){
        length = i_length;
        num = i_num;
        turn = 0;
        best_guess.w_hits = 0;
        best_guess.b_hits = 0;
        brute = brute_approach();
        create_soln_vector();
        new_testing_number = true;
        index_counter = 0;
    }

    /// this member function creates an attempt to find the right code
    /// this version generates a random attempt, then checks whether
    /// it has already been tried before and was not the right solution
    /// (if that's the case it generates another attempt etc)
    /// this is clearly still far from being a good implementation!
    void create_attempt(std::vector<int>& attempt){
      bool ready = false;

      if(!brute) {
        if(turn==0) {
          first_move(attempt, length, num);
          for(int i = 0; i < length; i++) {
            best_guess.attempted_sequence.push_back(attempt[i]);
          }
        }
        else {
          while(!ready){

              bool found = false;
              create_next_attempt(attempt);

              //check this hasn't been tried before
              for(int i = 0; i < not_correct.size() && !found; i++){
                  if(attempt == not_correct[i].attempted_sequence){
                      found = true;
                  }
              }

              if(found){
                  attempt.clear();
              }
              else{
                  ready = true;
              }
          }
        }
      }
      else {
        bool keep_trying = false;

        for(int i = 0; i < brute_solution.size(); i++) {
          if(brute_solution[i] == -1) {
            keep_trying = true;
          }
        }

        if(keep_trying) {
          if(turn < num) {
            for(int i = 0; i < length; i++) {
              attempt.push_back(turn);
            }
          }
          else{

            if ((testing_nums_placed == testing_num_appearances) && (filling_nums_placed == filling_num_appearances)) {
              new_testing_number = true;
            }
            if(index_counter >= (length-1)) {
              new_testing_number = true;
            }

            if(new_testing_number) {
              bool testing_num_found = false;
              bool filling_num_found = false;
              complete = false;
              testing_num_appearances = 0;
              filling_num_appearances = 0;
              testing_nums_placed = 0;
              filling_nums_placed = 0;

              if(turn==num) {
                testing_num = soln_values[0];
                for(int i = 0; i < soln_values.size(); i++) {
                  if((soln_values[i]==testing_num) && (soln_values[i]!=soln_values[i+1])) {
                    filling_num = soln_values[i+1];
                  }
                }

              }

              if(turn!=num) { //get new testing number
                for(int i = 0; i < soln_values.size(); i++) {
                  if(!testing_num_found) {
                    if((filling_num==soln_values[i]) && (soln_values[i]!=soln_values[i+1])) {
                      testing_num = soln_values[i+1];
                      testing_num_found = true;
                    }
                  }
                }

                for(int i = 0; i < soln_values.size(); i++) {
                  if(!filling_num_found) {
                    if((testing_num==soln_values[i]) && (soln_values[i]!=soln_values[i+1])) {
                      filling_num = soln_values[i+1];
                      filling_num_found = true;
                    }
                  }
                }

                if(testing_num == soln_values[soln_values.size() - 1]) {
                  complete = true;
                }

              }
              for(int i = 0; i < soln_values.size(); i++) {
                if(soln_values[i]==filling_num) {
                  filling_num_appearances++;
                }
                if(soln_values[i]==testing_num) {
                  testing_num_appearances++;
                }
              }

            }
            create_next_attempt_long(attempt);

          }
        }

        else {
          for(int i = 0; i < brute_solution.size(); i++) {
            attempt.push_back(brute_solution[i]);
          }
        }

      }

      turn++;
   }

   void create_next_attempt(std::vector<int>& attempt) {
     //generate next attempt based on best guess so far
     std::vector<bool> current_attempt_pos_flags;
     std::vector<bool> potential_attempt_pos_flags;
     std::vector<int> potential_attempt;
     bool available = false;
     bool fit = false;

     while(!fit) {
       potential_attempt.clear();
       current_attempt_pos_flags.clear();
       potential_attempt_pos_flags.clear();
       for(int i = 0; i < length; i++) { //initialise
         current_attempt_pos_flags.push_back(false);
         potential_attempt_pos_flags.push_back(false);
         potential_attempt.push_back(-1);
       }

       for(int hits = 0; hits < best_guess.b_hits; hits++) { //alter "black hits"
         int suspected_black_hit_location;
         available = false;

         while(!available) {
           suspected_black_hit_location = randn(length);
           if((!current_attempt_pos_flags[suspected_black_hit_location]) && (!potential_attempt_pos_flags[suspected_black_hit_location])) {
             available = true;
           }
         }

         potential_attempt[suspected_black_hit_location] = best_guess.attempted_sequence[suspected_black_hit_location];
         current_attempt_pos_flags[suspected_black_hit_location] = true;
         potential_attempt_pos_flags[suspected_black_hit_location] = true;
       }

       for(int hits = 0; hits < best_guess.w_hits; hits++) { //alter "white hits"
         int new_white_hit_location;
         int suspected_white_hit_location;
         bool chances = true;
         available = false;

         new_white_hit_location = randn(length);

         while(potential_attempt[new_white_hit_location] != -1)
             new_white_hit_location = randn(length);

         suspected_white_hit_location = randn(length);

         while(((current_attempt_pos_flags[suspected_white_hit_location] == true ||
           best_guess.attempted_sequence[new_white_hit_location] == best_guess.attempted_sequence[suspected_white_hit_location]) && chances)
           || new_white_hit_location == suspected_white_hit_location) {

             suspected_white_hit_location = randn(length);
             potential_attempt_pos_flags[suspected_white_hit_location] = true;

             //avoid infinite loop and start again
             chances = false;
             for(int i = 0; i < length; i++)
                 if(potential_attempt_pos_flags[i] == false)
                     chances = true;
         }

         current_attempt_pos_flags[suspected_white_hit_location] = true;
         potential_attempt[new_white_hit_location] = best_guess.attempted_sequence[suspected_white_hit_location];
       }

       //fill in -1 values in potential_attempt
       for(int i = 0; i < length; i++) {

         if(potential_attempt[i] == -1) {

           bool valid = false;
           while(!valid) {
             valid = true;
             potential_attempt[i] = randn(num);

             for(int n = 0; n < black_listed_values.size(); n++) {
               if(potential_attempt[i]==black_listed_values[n]) {
                 valid = false;
               }
             }
             if(potential_attempt[i] == best_guess.attempted_sequence[i]) {
               valid = false;
             }

             for(int n = 0; n < potential_attempt.size(); n++) {
               if(!current_attempt_pos_flags[n] && best_guess.attempted_sequence[n]==potential_attempt[i]) {
                 if(num!=2) {
                   valid = false;
                 }
               }
             }

           }

         }
       }

       fit = calculate_fitness(potential_attempt, not_correct);
     }

     for(int i = 0; i < length; i++) {
       attempt.push_back(potential_attempt[i]);
     }

   }

    void create_next_attempt_long(std::vector<int>& attempt) {
      for(int i = 0; i < length; i++) {
        attempt.push_back(-1);
      }

      if(complete) {
        for(int i = 0; i < length; i++) {
          attempt[i] = brute_solution[i];
          if(brute_solution[i] == -1) {
            attempt[i] = testing_num;
          }
        }
      }
      else {
        if(new_testing_number) {
          new_testing_number = false;
          index_counter = 0;
        }
        else {
          index_counter++;
        }

        while(brute_solution[index_counter]!=-1) {
          index_counter++;
        }

        attempt[index_counter] = testing_num;

        for(int i = 0; i < length; i++) {
          if(attempt[i]==-1) {
            attempt[i] = filling_num;
          }
        }
      }

    }

    void learn(std::vector<int>& attempt, int black_hits, int white_hits){
      //learns whether an attempt is correct or not

      //learns whether the attempted numbers could be in the sequence
      if(white_hits == 0 && black_hits == 0) {
        for(int i = 0; i < attempt.size(); i++) {
          black_listed_values.push_back(attempt[i]);
        }
      }

      if(!brute) {
        if(black_hits != attempt.size()){
          guess incorrect_guess;
          for(int i = 0; i < length; i++) {
            incorrect_guess.attempted_sequence.push_back(attempt[i]);
          }
          incorrect_guess.b_hits = black_hits;
          incorrect_guess.w_hits = white_hits;
          not_correct.push_back(incorrect_guess);
        }

        //overwrite best guess if latest guess is better
        if((black_hits + white_hits) > (best_guess.b_hits + best_guess.w_hits) ||
        ((black_hits + white_hits) == (best_guess.b_hits + best_guess.w_hits)) && (black_hits > best_guess.b_hits)) {
          best_guess.attempted_sequence.clear();
          for(int i=0; i<attempt.size(); i++) {
            best_guess.attempted_sequence.push_back(attempt[i]);
          }
          best_guess.b_hits = black_hits;
          best_guess.w_hits = white_hits;
        }
      }
      else {
        if(turn <= num) {
          for(int i = 0; i < black_hits; i++) {
            soln_values.push_back(turn - 1);
          }
        }

        else{

          if(white_hits==0) {
            brute_solution[index_counter] = testing_num;
            testing_nums_placed++;
          }

          else if(black_hits < filling_num_appearances) {
            brute_solution[index_counter] = filling_num;
            filling_nums_placed++;
          }
        }

      }


    }

    bool brute_approach() {
      if((pow(num, length) > pow(10, 10)) || length>10) { //higher than a 10x10 grid
        return true;
      }
      return false;
    }

    void create_soln_vector() {
      for(int i = 0; i < length; i++) {
        brute_solution.push_back(-1);
      }
    }

    std::vector< guess > not_correct;
    std::vector<int> black_listed_values;
    std::vector<int> soln_values;
    std::vector<int> brute_solution;
    guess best_guess;

    int length;
    int num;
    int turn;
    int testing_num;
    int testing_num_appearances;
    int filling_num;
    int filling_num_appearances;
    int index_counter;
    int testing_nums_placed;
    int filling_nums_placed;

    bool brute;
    bool new_testing_number;
    bool complete;

};

/// the main is the same as in the previous example

int main(){

    set_random_seed();

    int length, num;
    std::cout << "enter length of sequence and number of possible values:" << std::endl;
    std::cin >> length >> num;

    mm_solver solver;
    solver.init(length, num);

    mm_code_maker maker;
    maker.init(length, num);
    maker.generate_sequence();

    int black_hits=0, white_hits=0;
    int attempts_limit = 5000;
    int attempts = 0;
    while((black_hits < length) && (attempts < attempts_limit)){
        std::vector<int> attempt;
        solver.create_attempt(attempt);
        maker.give_feedback(attempt, black_hits, white_hits);
        std::cout << "attempt: " << std::endl;
        for(int i = 0; i < attempt.size(); i++){
            std::cout << attempt[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "black pegs: " << black_hits << " " << " white pegs: " << white_hits << std::endl;
        solver.learn(attempt, black_hits, white_hits);
        attempts++;
    }

    if(black_hits == length){
        std::cout << "the solver has found the sequence in " << attempts << " attempts" << std::endl;
    }
    else{
        std::cout << "after " << attempts << " attempts still no solution" << std::endl;
    }
    std::cout << "the sequence generated by the code maker was:" << std::endl;
    for(int i = 0; i < maker.sequence.size(); i++){
        std::cout << maker.sequence[i] << " ";
    }
    std::cout << std::endl;
    return 0;
}

void set_random_seed(){
    std::srand(std::time(0));
}

int randn(int n){
    return std::rand() % n;
}

void first_move(std::vector<int> &attempt, int len, int num) {
  for(int i = 0; i < (len/2); i++) {
    attempt.push_back(0);
  }
  for(int i = (len/2); i < len; i++) {
    if(num > 1) {
      attempt.push_back(1);
    }
    else {
      attempt.push_back(0);
    }
  }
}

bool calculate_fitness(std::vector<int> possible_soln, std::vector<guess> previous_guesses) {
  for(int m = 0; m < previous_guesses.size(); m++) {
    int black_hits = 0;
    int white_hits = 0;

    std::vector<int> possible_soln_copy;
    for(int a = 0; a < possible_soln.size(); a++){
        possible_soln_copy.push_back(possible_soln[a]);
    }

    for(int i = 0; i < possible_soln.size(); i++) {
      if(previous_guesses[m].attempted_sequence[i] == possible_soln[i]) {
        black_hits++;
      }

      bool remove = false;
      int remove_index;
      for(int n = 0; n < possible_soln_copy.size(); n++) {
        if(previous_guesses[m].attempted_sequence[i] == possible_soln_copy[n]) {
          white_hits++;
          remove = true;
          remove_index = n;
          break;
        }
      }
      if(remove) {
        possible_soln_copy.erase(possible_soln_copy.begin()+remove_index);
      }
    }

    white_hits = white_hits - black_hits;
    if((black_hits != previous_guesses[m].b_hits) || (white_hits != previous_guesses[m].w_hits)) {
      return false;
    }
  }
  return true;


}
