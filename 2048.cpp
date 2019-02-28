#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <iomanip>

int twod_to_oned(int row, int col, int rowlen);
void print_grid(const std::vector<int>& v);
bool proc_num(std::vector<int>& v, int bi, int ei);
bool make_move(std::vector<int>& v);
void rotate_anti_clock(std::vector<int>& v);
bool game_over(const std::vector<int>& v);
void insert_two(std::vector<int>& v);

int main() {

	std::vector<int> v;
	std::string filename;
	int side = 4;

	std::cout << "enter initial configuration file name:" << std::endl;
	std::cin >> filename;

	std::ifstream infile;
	infile.open(filename.c_str());

	if (!infile.is_open()) {
		std::cout << "file not found, using default start configuration" << std::endl;
		int i = 0;
		while (i < 15) {
			v.push_back(0);
			i++;
		}
		v.push_back(2); //pushback of 15 zeros and a 2 at the end, creates default 4x4 grid
	}
	else {
		int tmp;

		while (infile >> tmp) {
			v.push_back(tmp);
		}

		side = std::sqrt(v.size());
	}

	std::string move;
	bool valid = true;

	print_grid(v);

	while(!game_over(v)) {

		std::cin >> move;

		if(move=="a") {
			valid = make_move(v);
		}
		else{
			rotate_anti_clock(v);
			if(move=="w") {
				valid = make_move(v);
			}
			rotate_anti_clock(v);
			if(move=="d") {
				valid = make_move(v);
			}
			rotate_anti_clock(v);
			if(move=="s") {
				valid = make_move(v);
			}
			rotate_anti_clock(v);
		}

		if(valid) {
			insert_two(v);
			print_grid(v);
		}

	}

	std::cout<<"game over"<<std::endl;

	return 0;
}

int twod_to_oned(int row, int col, int rowlen) {
	return row * rowlen + col;
}

void print_grid(const std::vector<int>& v) {
	int side = std::sqrt(v.size());
	std::cout << std::endl;
	for (int i = 0; i < side; i++) {
		for (int j = 0; j < side; j++) {
			std::cout << v[twod_to_oned(i, j, side)] << " \t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

bool proc_num(std::vector<int>& v, int bi, int ei) {
  std::vector<int> va;
  std::vector<int> v_compare;
  for(int i = bi; i < ei; i++) {
    if(v[i] != 0) {
      va.push_back(v[i]);
    }
    v_compare.push_back(v[i]);
  }

	if(va.empty()) {
		return false;
	}

  for(int i = 0; i < va.size()-1; i++) {
    if(va[i] == va[i+1]) {
      va[i] = va[i]*2;
      va.erase(va.begin() + (i+1));
    }
  }

  int c = ei - bi; //which indexes to insert va
  int z = c - va.size(); //how many zeros to add

  for(int i = 0; i < z; i++) {
    va.push_back(0);
  }

  if(va==v_compare) {
    return false;
  }

  int n = 0; //insert back into v
  for(int i = bi; i < ei; i++) {
    v[i] = va[n];
    n++;
  }

  return true;
}

void rotate_anti_clock(std::vector<int>& v) {
  std::vector<int> tmp_v;
  int side = std::sqrt(v.size());
  int i = 0;

  while(i < v.size()) {
    for(int col = side-1; col >= 0; col--) {
      for(int row = 0; row < side; row++) {
        tmp_v.push_back(v[twod_to_oned(row, col, side)]);
        i++;
      }
    }
  }

  v.swap(tmp_v);
}

bool game_over(const std::vector<int>& v) { //return true if game over
  int side = std::sqrt(v.size());

  for(int i = 0; i < v.size(); i++) {
    if(v[i]==0) {
      return false;
    }
  }
  for(int i = 0; i < v.size()-1; i++) {
    if(v[i] == v[i+1]) {
      return false;
    }
  }
  for(int i = 0; i < v.size()-side; i++) {
    if(v[i]==v[i+side]) {
      return false;
    }
  }
  return true;
}

bool make_move(std::vector<int>& v) {
	int side = std::sqrt(v.size());
	bool rv = false;
	for(int i = 0; i < v.size(); i+=side) {
		bool ret = proc_num(v, i, i+side);
		if(ret) {
			rv = true;
		}
	}
	return rv;
}

void insert_two(std::vector<int>& v) {
	std::vector<int> zero_indexes;
	int rand_zero = 0;

	for(int i = 0; i < v.size(); i++) {
		if(v[i]==0) {
			zero_indexes.push_back(i);
		}
	}

	if(zero_indexes.size()>1) {
		rand_zero = std::rand() % zero_indexes.size()-1;
	}

	v[zero_indexes[rand_zero]] = 2;
}
