#include <iostream>
#include <fstream>
#include <ctime>
#include<cstring>
#define item_number  500
#define minsupport   60
//157
using namespace std;

typedef struct set_node
{
	int *set_name;
	int count;
	struct set_node *next;
	struct set_node *leftnext;
	struct set_node *rightnext;

} item_set;

item_set *gen_itrm_c1c2(int min_support);
item_set *gen_item_c3tocx(item_set *l_set_header);
item_set *couter_item_and_cut(item_set *c_set_header, int min_support);

char *data_scanner(char *tmp, char *source);
void readfile(char* file_name);


int layer = 0;
int terminate_flag = 0;
int large_set_counter = 0;
int sum = 0;

char file_name[] = "test.txt";
item_set *recordrear;
//--------------------------
item_set *ptr, *l_ptr;
int main()
{
	puts("hello");
	clock_t start, finish, now1;
	double duration;
	start = clock();
	l_ptr = gen_itrm_c1c2(minsupport);
	now1 = clock();

	while (1)
	{

		if (terminate_flag == 1) break;

		ptr = gen_item_c3tocx(l_ptr);
		l_ptr = ptr;
		if (terminate_flag == 1) break;
		ptr = couter_item_and_cut(l_ptr, minsupport);
		l_ptr = ptr;
		//cout << large_set_counter << endl;
	}
	finish = clock();
	cout << "amount of large set : " << large_set_counter << endl;
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "run time : " << duration << " seconds." << endl;
	cout << "aaaa";
	system("pause");
}


item_set *gen_itrm_c1c2(int min_support) // this funtion will be count l1 and l2
{

	int flag = 0, ccount = 0, comput = 0;
	int item[item_number] = { 0 };


	large_set_counter = 0;
	fstream infile;
	infile.open(file_name, ios::in);
	char data_row[512];
	char *scanner;
	char numchar[8];
	int real_num = 0;
	int count = 0;
	//int *item = new int[item_number];
	for (int i = 0; i < item_number; i++) item[i] = 0;
	data_row[0] = '\0';
	while (!infile.eof())	/*this loop convert string to a number array , i'm sure this loop have no bug.*/
	{
		infile.getline(data_row, 512, '\n');
		scanner = data_row;
		numchar[0] = '\0';
		while (*scanner != '\0')
		{
			scanner = data_scanner(numchar, scanner);
			if (*scanner == ',') scanner += 1;
			real_num = atoi(numchar);
			item[real_num] += 1;
			numchar[0] = '\0';
		}
		data_row[0] = '\0';
	}

	for (int i = 0; i < item_number; i++)
	{
		if (item[i] < min_support) item[i] = 0;
		else
		{
			count += 1;
			large_set_counter += 1;
		}
	}
	int *candidate_array = new int[count];
	int candidate_number = 0;
	for (int i = 0; i < item_number; i++)
	{
		if (item[i] != 0)
		{
			candidate_array[candidate_number] = i;
			candidate_number += 1;
		}
		else;
	}
	item_set *c_set_header = NULL;
	item_set *c_set_rear = NULL;
	item_set *c_set_builder;
	item_set *c_set_com_rear;

	c_set_builder = new item_set;
	c_set_header = c_set_builder;
	c_set_rear = c_set_builder;
	c_set_builder->next = NULL;
	c_set_builder->leftnext = NULL;
	c_set_builder->rightnext = NULL;
	c_set_builder->set_name = new int[2];

	for (int i = 0; i < count - 1; i++) /*this loop will generate c2*/
	{
		for (int j = i + 1; j < count; j++)
		{
			c_set_builder = new item_set;
			c_set_rear->next = c_set_builder;
			c_set_rear->rightnext = c_set_builder;
			c_set_builder->leftnext = c_set_rear;

			c_set_builder->set_name = new int[2];
			c_set_builder->count = 0;
			c_set_builder->set_name[0] = candidate_array[i];
			c_set_builder->set_name[1] = candidate_array[j];
			c_set_com_rear = c_set_rear = c_set_builder;
			c_set_builder->next = NULL;
			c_set_builder->rightnext = NULL;
			ccount++;
		}
	}

	c_set_builder = c_set_header;
	c_set_header = c_set_header->next;

	item_set *l_set_header = c_set_header;
	item_set *l_set_pointer = c_set_header;

	infile.close();
	infile.clear();

	int item_map[item_number] = { 0 };
	real_num = 0;
	flag = 0;

	//large_set_counter = 0;
	//	fstream infile;
	infile.open(file_name, ios::in);

	data_row[0] = '\0';
	while (!infile.eof())	/*this loop convert string to a number array , i'm sure this loop have no bug.*/
	{
		infile.getline(data_row, 512, '\n');
		scanner = data_row;
		numchar[0] = '\0';
		while (*scanner != '\0')
		{
			scanner = data_scanner(numchar, scanner);
			if (*scanner == ',') scanner += 1;
			real_num = atoi(numchar);
			item_map[real_num] = 1;
			numchar[0] = '\0';
		}
		data_row[0] = '\0';
		l_set_pointer = l_set_header;
		comput = 0;
		c_set_com_rear = c_set_rear;
		while (comput != ccount / 2)
		{
			int round = 0;
			if (item_map[l_set_pointer->set_name[0]] == 1 && item_map[l_set_pointer->set_name[1]] == 1)
			{
				l_set_pointer->count += 1;
			}
			if (item_map[c_set_com_rear->set_name[0]] == 1 && item_map[c_set_com_rear->set_name[1]] == 1)
			{
				c_set_com_rear->count += 1;
			}

			l_set_pointer = l_set_pointer->rightnext;
			c_set_com_rear = c_set_com_rear->leftnext;
			comput++;
		}
		memset(item_map, 0, sizeof(item_map));
	}


	item_set *l_set_last=NULL, *l_set_lastrear;
	l_set_pointer = l_set_header;
	c_set_com_rear = c_set_rear;
	comput = 0;
	while (l_set_pointer != NULL)
	{

		if (l_set_pointer->count < min_support)
		{
			if (l_set_pointer == l_set_header)
			{
				l_set_header = l_set_pointer->next;
				//delete l_set_pointer;
				l_set_pointer = l_set_header;
			}
			else
			{
				l_set_last->next = l_set_pointer->next;
				//delete l_set_pointer;
				l_set_pointer = l_set_last->next;
			}
		}
		else
		{
			l_set_last = l_set_pointer;
			large_set_counter += 1;
			l_set_pointer = l_set_pointer->next;
		}
	}

	layer = 2;

	l_set_pointer = l_set_header;
	infile.close();
	infile.clear();
	return l_set_header;
}


//this function will use layer to find next level set
//this function construsts a new c_set_list
//combine_array are a dynamic integer point array
item_set *gen_item_c3tocx(item_set *l_set_header)
{
	/* construst posible c_set_list */
	item_set *new_c_set_list_header = NULL;
	item_set *new_c_set_list_rear = NULL;
	item_set *new_c_set_builder = NULL;
	item_set *l_set_pointer, *l_set_finder;
	new_c_set_builder = new item_set;
	new_c_set_list_header = new_c_set_builder;
	new_c_set_list_rear = new_c_set_builder;
	new_c_set_builder->next = NULL;
	new_c_set_builder->leftnext = NULL;
	new_c_set_builder->rightnext = NULL;
	l_set_pointer = l_set_header;
	l_set_finder = l_set_header;
	bool check_point = 1;
	int ppd;
	int count = 0;
	sum = 0;
	while (l_set_pointer != NULL)
	{
		l_set_finder = l_set_pointer->next;
		while (l_set_finder != NULL)
		{
			count = 0;

			for (ppd = 0; ppd <layer - 1; ppd++)
			{
				if (l_set_pointer->set_name[ppd] != l_set_finder->set_name[ppd]) break;
				count++;
			}
			if (count == layer - 1)
			{
				new_c_set_builder = new item_set;
				new_c_set_list_rear->next = new_c_set_builder;
				new_c_set_list_rear->rightnext = new_c_set_builder;
				new_c_set_builder->leftnext = new_c_set_list_rear;
				new_c_set_builder->count = 0;
				recordrear = new_c_set_list_rear = new_c_set_builder;
				new_c_set_builder->next = NULL;
				new_c_set_builder->rightnext = NULL;
				new_c_set_builder->set_name = new int[layer + 1];
				new_c_set_builder->set_name[layer / 2] = l_set_pointer->set_name[layer / 2];
				sum++;

				if (layer % 2 == 0)
				{
					new_c_set_builder->set_name[layer / 2 + 1] = l_set_pointer->set_name[layer / 2 + 1];
				}

				for (int q = 0; q <layer / 2; q++){
					new_c_set_builder->set_name[q] = l_set_pointer->set_name[q];
					new_c_set_builder->set_name[layer - q - 1] = l_set_pointer->set_name[layer - q - 1];
					// cout<<l_set_pointer->set_name[q]<<" "<<l_set_finder->set_name[q]<<endl;
				}

				new_c_set_builder->set_name[layer] = l_set_finder->set_name[layer - 1];
			}
			else break;
			l_set_finder = l_set_finder->next;
		}
		l_set_pointer = l_set_pointer->next;
	}

	new_c_set_builder = new_c_set_list_header;
	new_c_set_list_header = new_c_set_list_header->next;
	//delete new_c_set_builder;

	/* now we remove some illegal node */
	l_set_pointer = l_set_header;
	item_set *c_set_police = new_c_set_list_header;
	item_set *c_set_last = NULL;
	//cout << "now enter while( c_set_police != NULL )" << endl;
	layer += 1;
	if (new_c_set_list_header == NULL) terminate_flag = 1;
	// cout<<recordrear<<endl;
	return new_c_set_list_header;
}


item_set *couter_item_and_cut(item_set *c_set_header, int min_support)
{

	item_set *c_set_ptr = c_set_header;
	item_set *c_set_com_rear = recordrear;
	int count = 0, comput = 0;
	int flag = 0;

	int item_map[item_number] = { 0 };
	int real_num = 0;
	int check1 = 0, check2 = 0;


	fstream infile;
	infile.open(file_name, ios::in);
	char data_row[512];
	char *scanner;
	char numchar[8];
	data_row[0] = '\0';
	while (!infile.eof())	/*this loop convert string to a number array , i'm sure this loop have no bug.*/
	{
		infile.getline(data_row, 512, '\n');
		scanner = data_row;
		numchar[0] = '\0';
		while (*scanner != '\0')
		{
			scanner = data_scanner(numchar, scanner);
			if (*scanner == ',') scanner += 1;
			real_num = atoi(numchar);
			item_map[real_num] = 1;
			numchar[0] = '\0';
		}
		data_row[0] = '\0';
		c_set_ptr = c_set_header;
		comput = 0;
		c_set_com_rear = recordrear;
		while (comput <= sum / 2)
		{
			int i;
			int a = 0, b = 0;
			for (i = 0; i <= layer / 2; i++)
			{

				if (item_map[c_set_ptr->set_name[i]] == 0 || item_map[c_set_ptr->set_name[layer - i - 1]] == 0)
					a = 1;
				//if(item_map[c_set_ptr->set_name[layer-i-1]]==0)
				if (item_map[c_set_com_rear->set_name[i]] == 0 || item_map[c_set_com_rear->set_name[layer - i - 1]] == 0)
					b = 1;
				if (a == 1 && b == 1)
					break;

			}

			if (a == 0 && b == 0)
			{
				if (c_set_ptr == c_set_com_rear)
				{
					c_set_ptr->count += 1;
					break;
				}
				c_set_ptr->count += 1;
				c_set_com_rear->count += 1;

			}
			if (a == 0 && b == 1)
			{
				if (c_set_ptr == c_set_com_rear)
				{
					c_set_ptr->count += 1;
					break;
				}

				c_set_ptr->count += 1;

			}
			if (a == 1 && b == 0)
			{
				if (c_set_ptr == c_set_com_rear)
				{
					c_set_ptr->count += 1;
					break;
				}
				//c_set_ptr->count += 1;
				c_set_com_rear->count += 1;

			}

			//c_set_ptr = c_set_ptr->next;
			c_set_ptr = c_set_ptr->rightnext;
			c_set_com_rear = c_set_com_rear->leftnext;
			comput++;
		}

		memset(item_map, 0, sizeof(item_map));
	}

	c_set_ptr = c_set_header;
	item_set *l_set_header = c_set_header;
	item_set *c_set_last;
	item_set *l_set_rear = recordrear;
	while (c_set_ptr != l_set_rear)
	{
		if (c_set_ptr->count < min_support)
		{
			if (c_set_ptr == l_set_header)
			{
				l_set_header = c_set_ptr->next;
				c_set_ptr = l_set_header;
			}
			else
			{
				c_set_last->next = c_set_ptr->next;
				c_set_ptr = c_set_last->next;
			}
		}
		else
		{
			large_set_counter += 1;
			c_set_last = c_set_ptr;
			c_set_ptr = c_set_ptr->next;
		}

		if (c_set_ptr != l_set_rear){
			if (l_set_rear->count < min_support)
			{
				if (l_set_rear == recordrear)
				{
					recordrear = l_set_rear->leftnext;
					recordrear->next = NULL;
					recordrear->rightnext = NULL;
					l_set_rear = recordrear;
				}
				else
				{
					l_set_rear->leftnext->rightnext = l_set_rear->rightnext;

					l_set_rear->rightnext->leftnext = l_set_rear->leftnext->rightnext;
					l_set_rear->leftnext->next = l_set_rear->next;

					l_set_rear = l_set_rear->leftnext;

				}
			}
			else
			{
				large_set_counter += 1;

				l_set_rear = l_set_rear->leftnext;
			}
		}

	}
	if (c_set_ptr->count < min_support)
	{
		c_set_ptr = c_set_ptr->next;
	}
	else
	{
		large_set_counter += 1;
	}

	infile.close();
	infile.clear();
	if (l_set_header == NULL)
		terminate_flag = 1;
	return l_set_header;
}

/* scan data to tmp array , when find ',' or '\0' will stop, and return scanner's pointer */
//									already tested (?)
char *data_scanner(char *tmp, char *source)
{
	char *ptr = source;
	while (1)
	{
		if (*ptr == ',' || *ptr == '\0') break;
		else
		{
			if (*ptr != ' ') strncat(tmp, ptr, 1);
			ptr += 1;
		}
	}
	return ptr;
}
/* scan data to tmp array , when find ',' or '\0' will stop, and return scanner's pointer */
//									already tested (?)
