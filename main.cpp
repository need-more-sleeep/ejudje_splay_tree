#include <iostream>
#include <string>
#include <vector>
#include <regex>
struct node {
	long long int key;
	std::string data;
	node* left = nullptr;
	node* right = nullptr;


	node(long long int K, const std::string& V) :key(K), data(std::move(V)) {	}
};

class splay_tree {
	node* root = nullptr;
	int height;
	std::vector<std::string> path;

	node* splay(node* root, int key) {

		if (root == nullptr || root->key == key)
			return root;

		if (key < root->key)
		{
			if (root->left == nullptr) return root;

			if (root->left->key > key)
			{
				root->left->left = splay(root->left->left, key);

				root = right_rotate(root);
			}
			else if (root->left->key < key)
			{
				root->left->right = splay(root->left->right, key);

				if (root->left->right != nullptr)
					root->left = left_rotate(root->left);
			}
			if (root->left == nullptr) {
				return root;
			}
			else {
				return right_rotate(root);
			}
		}
		else
		{
			if (root->right == nullptr) return root;

			if (root->right->key > key)
			{
				root->right->left = splay(root->right->left, key);

				if (root->right->left != nullptr)
					root->right = right_rotate(root->right);
			}
			else if (root->right->key < key)
			{

				root->right->right = splay(root->right->right, key);
				root = left_rotate(root);
			}

			if (root->right == nullptr) {
				return root;
			}
			else {
				return left_rotate(root);
			}
		}
	}


	node* right_rotate(node* x) {
		node* y = x->left;
		x->left = y->right;
		y->right = x;
		return y;
	}
	node* left_rotate(node* x) {
		node* y = x->right;
		x->right = y->left;
		y->left = x;
		return y;
	}

	void print_level(const node* root_, long long int parent_key, int level = 0) {
		std::string str;
		if (root_ == root)
		{
			str = "[" + std::to_string(root_->key) + " " + root_->data + "]";
		}
		else {
			str = "[" + std::to_string(root_->key) + " " + root_->data + " " + std::to_string(parent_key) + "]";
		}

		if (path[level].empty()) {
			path[level] += str;
		}
		else {
			path[level] += " " + str;
		}
		if (root_->left != nullptr) {
			print_level(root_->left, root_->key, level + 1);
		}
		//дополняем _ слева
		else  if (root_->left == nullptr && height > level + 1) {
			for (int i = level + 1, k = 1; i < height; i++, k *= 2)
			{
				for (int j = 0; j < k; j++)
				{
					if (path[i].empty()) {
						path[i] += "_";
					}
					else {
						path[i] += " _";
					}
				}
			}
		}
		if (root_->right != nullptr) {
			print_level(root_->right, root_->key, level + 1);
		}
		//дополняем _ справа
		else  if (root_->right == nullptr && height > level + 1) {
			for (int i = level + 1, k = 1; i < height; i++, k *= 2)
			{
				for (int j = 0; j < k; j++)
				{
					if (path[i].empty()) {
						path[i] += "_";
					}
					else {
						path[i] += " _";
					}
				}
			}
		}

		--level;
	}


public:


	void add(long long int K,const std::string& V) {
		if (root == nullptr) {
			root = new node(K, V);
			return;
		}
		node* parent_temp = nullptr;
		node* temp = root;
		while (temp != nullptr) {
			parent_temp = temp;
			if (K < temp->key) {
				temp = temp->left;
			}
			else if (K > temp->key) {
				temp = temp->right;
			}
			else {
				std::cout << "error\n";
				root = splay(root, K);
				return;
			}
		}

		if (parent_temp != nullptr) {
			if (K < parent_temp->key) {
				parent_temp->left = new node(K, V);
				root= splay(root, K);
			}
			else {
				parent_temp->right = new node(K, V);
				root = splay(root, K);
			}
		}
	}


	void search(long long int K) {
		root = splay(root, K);
		if (root->key == K) {
			std::cout << "1 " << root->data << std::endl;
		}
		else {
			std::cout << "0" << std::endl;
		}
	}



	void print() {
		height = get_height(root);
		path.clear();
		path.resize(height);
		print_level(root, 0);
		for (size_t i = 0; i < path.size(); i++)
		{
			std::cout << path[i] << "\n";
		}
	}

	int get_height(const node* root)
	{
		if (root == NULL) {
			return 0;
		}
		else if (root->left == NULL && root->right == NULL) {
			return 1;
		}
		else if (get_height(root->left) > get_height(root->right))
			return get_height(root->left) + 1;
		else
			return get_height(root->right) + 1;

	}

	void set(long long int K, const std::string& V) {
		root = splay(root, K);
		if (root->key == K) {
			root->data = std::move(V);
		}
		else {
			std::cout << "error" << std::endl;
		}
	}

	void min() {
		node* curr = root;
		while (curr->left != nullptr) {
			curr = curr->left;
		}
		std::cout << curr->key << " " << curr->data << std::endl;
		root = splay(root, curr->key);
	}

	void max() {
		node* curr = root;
		while (curr->right != nullptr) {
			curr = curr->right;
		}
		std::cout << curr->key << " " << curr->data << std::endl;
		root = splay(root, curr->key);
	}

	void delete_node(long long int K) {
		root = splay(root, K);
		if (root->key == K) {
			//если есть левое поддерево
			if (root->left != nullptr) {
				node* curr = root->left;
				node* curr_parent = root;
				while (curr->right != nullptr) {
					curr = curr->right;
					curr_parent = curr_parent->right;
				}
				if (curr_parent != root) {
					curr_parent->right = nullptr;
					root->key = std::move(curr->key);
					root->data = std::move(curr->data);
				}
				else {
					root->key = std::move(curr->key);
					root->data = std::move(curr->data);
					root->left = std::move(curr->left);
				}
				
				//надо указатели перенести на лево и напрва
			}
			//иначе идем вправо или просто удаляем
			else {
				if (root->right == nullptr) {
					delete root;
				}
				else {
					node* curr = root->right;
					node* curr_parent = root;
					while (curr->left != nullptr) {
						curr = curr->left;
						curr_parent = curr_parent->left;
					}
					if (curr_parent != root) {
						curr_parent->left = nullptr;
						root->key = std::move(curr->key);
						root->data = std::move(curr->data);
					}
					else {
						root->key = std::move(curr->key);
						root->data = std::move(curr->data);
						root->right = curr->right;
					}
				}
			}

		}
		else {
			std::cout << "error" << std::endl;
		}
	}

	const node* get_root() const {
		return root;
	}
};


int main() {
	
	splay_tree t;

	std::regex regex_add("^add ([0-9]+) ([^ ]+)$");
	std::regex regex_set("^set ([0-9]+) ([^ ]+)$");

	std::regex regex_delete("^delete ([0-9]+)$");
	std::regex regex_search("^search ([0-9]+)$");

	std::regex regex_min("^min$");
	std::regex regex_max("^max$");

	std::regex regex_print("^print$");

	std::smatch match;
	std::string str;


	while (std::getline(std::cin, str)) {
		if (str.empty()) {
			continue;
		}
		else if (std::regex_match(str, match, regex_add)) {
			t.add(std::stoll(match[1]),match[2]);
		}
		else if (std::regex_match(str, match, regex_set)) {
			t.set(std::stoll(match[1]), match[2]);
		}
		else if (std::regex_match(str, match, regex_delete)) {
			t.delete_node(std::stoll(match[1]));
		}
		else if (std::regex_match(str, match, regex_search)) {
			t.search(std::stoll(match[1]));
		}
		else if (std::regex_match(str, match, regex_min)) {
			t.min();
		}
		else if (std::regex_match(str, match, regex_max)) {
			t.max();
		}
		else if (std::regex_match(str, match, regex_print)) {
			t.print();
		}
		else {
			std::cout << "error\n";
		}
	}


	/*
	add 8 10
add 4 14
add 7 15
set 8 11
add 3 13
add 5 16
search 88
search 7
delete 5
print
	std::string s = "10";
	std::string s1 = "14";
	std::string s2 = "15";
	std::string s4 = "13";
	std::string s6 = "16";

	std::string s7 = "11";
	std::string s8 = "110";

	t.add(8, s);
	t.add(4, s1);
	t.add(7, s2);
	t.set(8, s7);
	t.add(3, s4);
	t.add(5, s6);
	t.search(88);
	t.search(7);
	//t.add(6, s7);
	t.delete_node(5);
	t.print();
	*/
}