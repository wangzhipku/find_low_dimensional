#include "CONNECTION.h"
#include <fstream>
#include <iostream>


using namespace std;


int classify_metal_maingroup(int &atomic_number)//0 rare gas,1 metal ,2 main group
{
	
	int j = 0, k = 0, m = 0;
	while (j < main_groupnum || k < metal_num || m < rare_gasnum)
	{
		if (rare_gas[m] == atomic_number)
		{
			return 0;
		}
		if (meatal_xuhao[k] == atomic_number)
		{
			return 1;
		}
		if (main_group_element[j] == atomic_number)
		{
			return 2;
		}


		if (meatal_xuhao[k] < atomic_number)
		{
			k++;
			continue;
		}
		if (rare_gas[m] < atomic_number)
		{
			m++;
			continue;
		}
		if (main_group_element[j] < atomic_number)
		{
			j++;
			continue;
		}
	}
	cout << "what's wrong!" << atomic_number << endl;
	cin.get();
	return -1;

}



cell::cell(char *name,element*e,vector<vector<double>>&max_ion,int flag) 
{
	int i, j, k;
	//cout << "expand the :" << cengshu << "layer" << endl;
	char temp[300];
	double x_pian = 0.0;
	double y_pian = 0.0;
	double z_pian = 0.0;
	//strcpy(wenjian, "atom1.config");
	FILE *in;
	in = fopen(name, "rt");
	//system("pause");
	if (in == NULL)
	{
		printf("error of rading atom.config!\n");
		printf("the filename is :%s\n", name);
		//cin.get();
		return;
	}
	fscanf(in, "%d", &num);
	if(flag==1)
		positive = new int[num];
	type = (int *)malloc(num * sizeof(int));
	letice = (double **)malloc(3 * sizeof(double *));
	for (i = 0; i < 3; i++)
	{
		letice[i] = (double *)malloc(3 * sizeof(double));
	}
	p = (double **)malloc(num * sizeof(double *));
	for (i = 0; i < num; i++)
	{
		p[i] = (double *)malloc(3 * sizeof(double));
	}
	real_position = (double ***)malloc(yanshen * sizeof(double **));
	for (i = 0; i < yanshen; i++)
	{
		real_position[i] = (double **)malloc(num * sizeof(double *));
		for (k = 0; k < num; k++)
			real_position[i][k] = (double *)malloc(3 * sizeof(double));
	}

	p_real = new double **[yanshen];
	for (i = 0; i < yanshen; i++)
	{
		p_real[i] = new double *[num];
		for (k = 0; k < num; k++)
		{
			p_real[i][k] = new double[3];
		}
	}
	if (flag == 1)
	{
		ridus = new double*[num];
		for (i = 0; i < num; i++)
		{
			ridus[i] = new double[2];
		}
	}
		
	while (fgets(temp, 300, in) != NULL)
	{
		if (strstr(temp, "VECTOR") != NULL || strstr(temp, "vector") != NULL)
			break;
	}
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			fscanf(in, "%lf", &letice[i][j]);
		}
		fgets(temp, 300, in);
	}	

	fgets(temp, 300, in);
	//cout << temp << endl;
	char line[10];
	for (i = 0; i < num; i++)
	{

		fscanf(in, "%d", &type[i]);
		fscanf(in, "%lf", &p[i][0]);
		fscanf(in, "%lf", &p[i][1]);
		fscanf(in, "%lf", &p[i][2]);
		if (flag == 1)
		{
			fscanf(in, "%s", line);
			if (line[0] == 'p')
			{
				positive[i] = 1;
				fscanf(in, "%lf", &ridus[i][0]);
			}
			else if (line[0] == 's')//读到s的时候说明是单质
			{
				fscanf(in, "%s", line);
				fscanf(in, "%lf", &ridus[i][0]);
			}
			else//如果是negative的话，就读两个ridus值
			{
				positive[i] = -1;
				fscanf(in, "%lf", &ridus[i][0]);//第一个是负价态对应的半径
				fscanf(in, "%lf", &ridus[i][1]);//对应正价态的半径
			}
		}
		

		fgets(temp, 300, in);
	}
	//int x_xishu = 0;
	//int y_xishu = 0;
	//int z_zishu = 0;

	//增加记录type的记录
	int temp_save[120] = { 0 };
	type_num = 0;
	for (i = 0; i < num; i++)
	{
		temp_save[type[i]]++;
	}
	for (i = 0; i < 120; i++)
	{
		if (temp_save[i] != 0)
			type_num++;
	}
	type_save = new int[type_num];
	j = 0;
	for (i = 0; i < 120; i++)
	{
		if (temp_save[i] != 0)
			type_save[j++] = i;
	}

	for (i = 0; i < yanshen; i++)
	{

		for (j = 0; j < num; j++)
		{
			//x_xishu = i/3;
			//y_xishu = i / 3;
			//z_zishu = (i % 9) / 3;

			real_position[i][j][0] = letice[0][0] * p[j][0] + letice[1][0] * p[j][1] + letice[2][0] * p[j][2] + (i % cengshu - ((cengshu - 1) / 2)) * letice[0][0] + (i % (cengshu * cengshu) / cengshu - ((cengshu - 1) / 2)) * letice[1][0] - (i / (cengshu * cengshu) - ((cengshu - 1) / 2)) * letice[2][0];
			real_position[i][j][1] = letice[0][1] * p[j][0] + letice[1][1] * p[j][1] + letice[2][1] * p[j][2] + (i % cengshu - ((cengshu - 1) / 2)) * letice[0][1] + (i % (cengshu * cengshu) / cengshu - ((cengshu - 1) / 2)) * letice[1][1] - (i / (cengshu * cengshu) - ((cengshu - 1) / 2)) * letice[2][1];
			real_position[i][j][2] = letice[0][2] * p[j][0] + letice[1][2] * p[j][1] + letice[2][2] * p[j][2] + (i % cengshu - ((cengshu - 1) / 2)) * letice[0][2] + (i % (cengshu * cengshu) / cengshu - ((cengshu - 1) / 2)) * letice[1][2] - (i / (cengshu * cengshu) - ((cengshu - 1) / 2)) * letice[2][2];
		}
	}
	for (i = 0; i < yanshen; i++)
	{
		for (j = 0; j < num; j++)
		{
			p_real[i][j][0] = (i % cengshu - ((cengshu - 1) / 2)) + p[j][0];
			p_real[i][j][1] = (i % (cengshu * cengshu) / cengshu - ((cengshu - 1) / 2)) + p[j][1];
			p_real[i][j][2] = -(i / (cengshu * cengshu) - ((cengshu - 1) / 2)) + p[j][2];
		}
	}

	if (flag == 2)
	{
		my_classify = new int[num];
		if_positive = new int [num];
		//开始按照之前的结果进行标定
		for (i = 0; i < num; i++)
		{
			my_classify[i] = classify_metal_maingroup(type[i]);
			
		}
		for (i = 0; i < num; i++)
		{
			//针对H和O的特殊情况
			if (type[i] == 1)
			{
				if_positive[i] = 1;
				for (j = 0; j < yanshen; j++)
				{
					for (k = 0; k < num; k++)
					{
						if (my_classify[k] == 1 && e[type[k]].electron_negativity<2.20)
						{
							double temp_dis = dis(real_position[13][i], real_position[j][k]);
							if (temp_dis <( max_ion[1][1]+max_ion[type[k]][0])*ridus_plus_factor)
							{
								if_positive[i] = -1;
								j = yanshen + 1;
								break;
							}
						}
					}
				}
			}
			/*else if (type[i] == 8)
			{
				if_positive[i] = -1;
				for (j = 0; j < num; j++)
				{
					if (type[j] == 9)
					{
						for (k = 0; k < yanshen; k++)
						{
							double temp_dis = dis(real_position[13][i],real_position[k][j]);
							if (temp_dis < OF_RULE)
							{
								if_positive[i] = 1;
								j = num + 1;
								break;
							}
						}
					}
				}
			}*/
			else
			{
				if (my_classify[i] == 0)
				{
					if_positive[i] = 0;
				}
				else if (my_classify[i] == 1)
				{
					if_positive[i] = 1;
				}
				else if (my_classify[i] == 2)
				{
					if_positive[i] = -1;
				}
			}
			//cout << atom_name[type[i]] << ":" << my_classify[i] <<"positive:"<< if_positive[i] << endl;

		}
	}








	fclose(in);
}

cell::cell(char *name)
{
	int i, j, k;
	//cout << "expand the :" << cengshu << "layer" << endl;
	char temp[300];
	double x_pian = 0.0;
	double y_pian = 0.0;
	double z_pian = 0.0;
	//strcpy(wenjian, "atom1.config");
	FILE *in;
	in = fopen(name, "rt");
	//system("pause");
	if (in == NULL)
	{
		printf("error of rading atom.config!\n");
		printf("the filename is :%s\n", name);
		//cin.get();
		return;
	}
	fscanf(in, "%d", &num);	
	type = (int *)malloc(num * sizeof(int));
	letice = (double **)malloc(3 * sizeof(double *));
	for (i = 0; i < 3; i++)
	{
		letice[i] = (double *)malloc(3 * sizeof(double));
	}
	p = (double **)malloc(num * sizeof(double *));
	for (i = 0; i < num; i++)
	{
		p[i] = (double *)malloc(3 * sizeof(double));
	}
	real_position = (double ***)malloc(yanshen * sizeof(double **));
	for (i = 0; i < yanshen; i++)
	{
		real_position[i] = (double **)malloc(num * sizeof(double *));
		for (k = 0; k < num; k++)
			real_position[i][k] = (double *)malloc(3 * sizeof(double));
	}

	p_real = new double **[yanshen];
	for (i = 0; i < yanshen; i++)
	{
		p_real[i] = new double *[num];
		for (k = 0; k < num; k++)
		{
			p_real[i][k] = new double[3];
		}
	}
	
	while (fgets(temp, 300, in) != NULL)
	{
		if (strstr(temp, "VECTOR") != NULL || strstr(temp, "vector") != NULL)
			break;
	}
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			fscanf(in, "%lf", &letice[i][j]);
		}
		fgets(temp, 300, in);
	}

	fgets(temp, 300, in);
	//cout << temp << endl;
	char line[10];
	for (i = 0; i < num; i++)
	{

		fscanf(in, "%d", &type[i]);
		fscanf(in, "%lf", &p[i][0]);
		fscanf(in, "%lf", &p[i][1]);
		fscanf(in, "%lf", &p[i][2]);
		fgets(temp, 300, in);
	}
	//int x_xishu = 0;
	//int y_xishu = 0;
	//int z_zishu = 0;

	//增加记录type的记录
	int temp_save[120] = { 0 };
	type_num = 0;
	for (i = 0; i < num; i++)
	{
		temp_save[type[i]]++;
	}
	for (i = 0; i < 120; i++)
	{
		if (temp_save[i] != 0)
			type_num++;
	}
	type_save = new int[type_num];
	j = 0;
	for (i = 0; i < 120; i++)
	{
		if (temp_save[i] != 0)
			type_save[j++] = i;
	}

	for (i = 0; i < yanshen; i++)
	{

		for (j = 0; j < num; j++)
		{
			//x_xishu = i/3;
			//y_xishu = i / 3;
			//z_zishu = (i % 9) / 3;

			real_position[i][j][0] = letice[0][0] * p[j][0] + letice[1][0] * p[j][1] + letice[2][0] * p[j][2] + (i % cengshu - ((cengshu - 1) / 2)) * letice[0][0] + (i % (cengshu * cengshu) / cengshu - ((cengshu - 1) / 2)) * letice[1][0] - (i / (cengshu * cengshu) - ((cengshu - 1) / 2)) * letice[2][0];
			real_position[i][j][1] = letice[0][1] * p[j][0] + letice[1][1] * p[j][1] + letice[2][1] * p[j][2] + (i % cengshu - ((cengshu - 1) / 2)) * letice[0][1] + (i % (cengshu * cengshu) / cengshu - ((cengshu - 1) / 2)) * letice[1][1] - (i / (cengshu * cengshu) - ((cengshu - 1) / 2)) * letice[2][1];
			real_position[i][j][2] = letice[0][2] * p[j][0] + letice[1][2] * p[j][1] + letice[2][2] * p[j][2] + (i % cengshu - ((cengshu - 1) / 2)) * letice[0][2] + (i % (cengshu * cengshu) / cengshu - ((cengshu - 1) / 2)) * letice[1][2] - (i / (cengshu * cengshu) - ((cengshu - 1) / 2)) * letice[2][2];
		}
	}
	for (i = 0; i < yanshen; i++)
	{
		for (j = 0; j < num; j++)
		{
			p_real[i][j][0] = (i % cengshu - ((cengshu - 1) / 2)) + p[j][0];
			p_real[i][j][1] = (i % (cengshu * cengshu) / cengshu - ((cengshu - 1) / 2)) + p[j][1];
			p_real[i][j][2] = -(i / (cengshu * cengshu) - ((cengshu - 1) / 2)) + p[j][2];
		}
	}	
	fclose(in);
}

save::save(int num)
{
	int i, j, k;
	sunbets = 0;
	save_list = new int *[num];
	for (i = 0; i < num; i++)
	{
		save_list[i] = new int[num];
	}
	for (i = 0; i < num; i++)
	{
		for (j = 0; j < num; j++)
		{
			save_list[i][j] = -1;
		}
	}
}
save::~save(void) //�������ǲ��걸�ģ�˼����ô���������ص�����ֵ
{
	int i = 0, j = 0;
	delete[] save_list;
	//cout << "the object is being destroied!" << endl;
}


void random_choose_two(int *org, int num, int **in)
{
	//从给定的org当中，任取两个，填充到in里面
	int i = 0, j = 0;
	//int total = num * (num - 1) / 2;
	int count = 0;
	for (i = 0; i < num; i++)
	{
		for (j = i + 1; j < num; j++)
		{
			in[count][0] = org[i];
			in[count][1] = org[j];
			//cout << in[count][0] << "," << in[count][1] << endl;
			count++;
		}
	}
	//cin.get();
}
int judge_falg_file(char *path)
{
	//成功返回1，失败返回0
	ifstream fin;
	char real[200];
	strcpy(real, path);
	strcat(real, "/flag");
	fin.open(real, ios::in);
	string temp;
	fin >> temp;
	fin.close();
	if (temp == "Different")
		return 1;
	else if (temp.find("All") != -1)
		return 0;
	else
	{
		cout << "wrong!flag file!" << endl;
		cin.get();
		return 0;
	}
}

void read_radius(ionic_radius *****ir, string& file_name1, string& file_name2)
{

	int i, j, k, l, m;
	int line_in;
	int coor_in;
	int val_in, structure_in, spin_state_in;
	double radius_in;
	char c;
	char str[5];
	char str_temp[500];
	FILE *in;
	int max = -9999;
	char atom[120][3] = { "D","H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th", "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt", "Ds", "Rg" };

	for (i = 0; i < 120; i++)
	{
		for (j = 0; j < 20; j++)
		{
			for (k = 0; k < 15; k++)
			{
				for (l = 0; l < 3; l++)
				{
					for (m = 0; m < 3; m++)
					{
						ir[i][j][k][l][m].atomic_num = i;
						ir[i][j][k][l][m].coor_num = k;
						ir[i][j][k][l][m].val_state = j - 8;
						ir[i][j][k][l][m].radius = -9999;
						ir[i][j][k][l][m].structure_type = -9999;
						ir[i][j][k][l][m].spin_stat = -9999;
					}
				}
			}
		}
	}
	in = fopen(file_name1.c_str(), "r");
	if (in == NULL)
	{
		printf("input_radius does not exist!\n");
		return;
	}
	c = '0';
	for (line_in = 0; line_in < 503; line_in++)
	{
		fscanf(in, "%s", str);
		for (i = 0; i < 120; i++)
		{
			if (str[0] == atom[i][0] && str[1] == atom[i][1] && str[1] == '\0')
			{
				break;
			}
			if (str[0] == atom[i][0] && str[1] == atom[i][1] && str[2] == atom[i][2] && str[2] == '\0')
			{
				break;
			}
		}
		if (i == 120)
		{
			printf("ERROR :%s\n", str);
			cout << line_in << endl;
			cin.get();
			return;
		}
		fscanf(in, "%d", &val_in);
		fscanf(in, "%d", &coor_in);
		fscanf(in, "%d", &structure_in);
		fscanf(in, "%d", &spin_state_in);
		fscanf(in, "%lf", &radius_in);
		ir[i][val_in + 8][coor_in][structure_in][spin_state_in].atomic_num = i;
		ir[i][val_in + 8][coor_in][structure_in][spin_state_in].val_state = val_in;
		ir[i][val_in + 8][coor_in][structure_in][spin_state_in].coor_num = coor_in;
		ir[i][val_in + 8][coor_in][structure_in][spin_state_in].spin_stat = spin_state_in;
		ir[i][val_in + 8][coor_in][structure_in][spin_state_in].radius = radius_in;
		ir[i][val_in + 8][coor_in][structure_in][spin_state_in].structure_type = structure_in;
		/*for (i=0;i<15;i++)
		{
			if (max<ir[i][val_in+8][i].radius)
			{
				max=ir[i][val_in+8][i].radius;
			}
		}
		ir[i][val_in+8][0].radius=max*1.1;*/
	}
	fclose(in);
	//继续读取我补充的信息
	ifstream fin;
	fin.open(file_name2, ios::in);
	if (!fin.is_open())
	{
		cout << "i can not find the file !" << file_name2 << endl;
		cin.get();
	}
	while (fin.peek() != EOF && fin.good())
	{
		fin >> str;
		//cout << str;
		for (i = 0; i < 120; i++)
		{
			if (str[0] == atom[i][0] && str[1] == atom[i][1] && str[1] == '\0')
			{
				break;
			}
			if (str[0] == atom[i][0] && str[1] == atom[i][1] && str[2] == atom[i][2] && str[2] == '\0')
			{
				break;
			}
		}
		if (i == 120)
		{
			//printf("ERROR :%s\n", str);
			//cout << file_name2 << endl;
			//cin.get();
			return;
		}
		fin >> val_in;
		fin >> coor_in;
		fin >> radius_in;
		ir[i][val_in + 8][coor_in][0][0].atomic_num = i;
		ir[i][val_in + 8][coor_in][0][0].coor_num = coor_in;
		ir[i][val_in + 8][coor_in][0][0].val_state = val_in;
		ir[i][val_in + 8][coor_in][0][0].spin_stat = 0;
		ir[i][val_in + 8][coor_in][0][0].structure_type = 0;
		ir[i][val_in + 8][coor_in][0][0].radius = radius_in;

	}
	fin.close();


	//做完这个需要补充一点事情，就是取出一个最大值用来预估配位数，我们把不同元素和价态的最大半径放到一个位置，以后方便取用
	for (i = 0; i < 120; i++)
	{
		for (j = 0; j < 20; j++)
		{
			for (k = 0; k < 15; k++)
			{
				for (l = 0; l < 3; l++)
				{
					for (m = 0; m < 3; m++)
					{
						if (max < ir[i][j][k][l][m].radius)
						{
							max = ir[i][j][k][l][m].radius;
						}
					}
				}
			}
			//每种情况取出来的最大值在这里赋值,放到对应的元素和价态，0配位，0结构，0自旋的半径下
			max = ir[i][j][0][0][0].radius;
			max = -9999;
		}
	}
	return;
}

//这里建立一个重载版本
void read_element(element *e, string& file_element_r, string& file_colvance, string& file_electronic_negativity, string& file_first_ionization_energy)
{
	int i, j, k;
	char atom_name[120][3] = { "D","H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th", "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt", "Ds", "Rg" };

	//先初始化信息
	for (i = 0; i < 120; i++)
	{
		strcpy(e[i].name, atom_name[i]);
		e[i].atomic_num = i;
		e[i].num_common_val = 0;
		e[i].num_metal_radius = 0;
		e[i].num_unusual_val = 0;
	}
	i = 0, j = 0, k = 0;
	int line_in;
	ifstream fin;
	int atom_num_in;
	double vdw_r_min_in, vdw_r_max_in;
	double cov_r;
	int num_metal_r;
	double *temp;
	char str[5];
	char str_temp[500];
	char atom[120][3] = { "D","H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th", "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt", "Ds", "Rg" };
	int num_common_in;
	int num_unsual_in;
	//读取共价键范德华键和金属键

	fin.open(file_element_r.c_str(), ios::in);
	if (!fin.is_open())
	{
		cout << "i can not find the file !" << file_element_r << endl;
		cin.get();
	}
	while (fin.peek() != EOF && fin.good())
	{
		fin >> atom_num_in;
		fin >> vdw_r_min_in;
		fin >> vdw_r_max_in;
		fin >> cov_r;
		fin >> num_metal_r;

		e[atom_num_in].metal_radius = new double[num_metal_r];
		double *temp = new double[num_metal_r];
		for (i = 0; i < num_metal_r; i++)
		{
			fin >> temp[i];
		}
		e[atom_num_in].atomic_num = atom_num_in;
		e[atom_num_in].vdw_radius_max = vdw_r_max_in;
		e[atom_num_in].vdw_radius_min = vdw_r_min_in;
		e[atom_num_in].cov_radius = cov_r;
		e[atom_num_in].num_metal_radius = num_metal_r;
		for (i = 0; i < num_metal_r; i++)
		{
			e[atom_num_in].metal_radius[i] = temp[i];
		}
		free(temp);
	}
	fin.close();


	////然后开始读取价态相关信息
	//fin.clear();
	//fin.open(file_colvance.c_str(), ios::in);
	//if (!fin.is_open())
	//{
	//	cout << "i can not find the file !" << file_colvance.c_str() << endl;
	//	cin.get();
	//}
	//while (fin.peek() != EOF && fin.good())
	//{
	//	fin >> atom_num_in;
	//	//printf("%d\n", atom_num_in);
	//	//fin >> num_common_in;
	//	fin >> e[atom_num_in].num_common_val;
	//	e[atom_num_in].common_val = new int[e[atom_num_in].num_common_val];
	//	for (i = 0; i < e[atom_num_in].num_common_val; i++)
	//	{
	//		fin >> e[atom_num_in].common_val[i];
	//	}
	//	fin >> e[atom_num_in].num_unusual_val;
	//	e[atom_num_in].unusual_val = new int[e[atom_num_in].num_unusual_val];
	//	for (i = 0; i < e[atom_num_in].num_unusual_val; i++)
	//	{
	//		fin >> e[atom_num_in].unusual_val[i];
	//	}
	//	//delete[]e[atom_num_in].common_val;
	//}
	//fin.close();

	//开始读取电负性相关信息
	string temp_test;
	fin.clear();
	fin.open(file_electronic_negativity.c_str(), ios::in);
	if (!fin.is_open())
	{
		cout << "i can not find the file !" << file_electronic_negativity << endl;
		cin.get();
	}
	while (fin.peek() != EOF && fin.good())
	{
		fin >> atom_num_in;
		//cout << atom_num_in << endl;
		fin >> str;
		//fin >> temp_test;
		//cout <<temp_test << endl;
		/*if (str[0] == '\0')
		{
			break;
		}*/
		fin >> e[atom_num_in].electron_negativity;
		//cout << e[atom_num_in].electron_negativity << endl;
		if (str[0] != atom[atom_num_in][0] || str[1] != atom[atom_num_in][1])
		{
			printf("ERROR atom name %s %s %d\n", str, atom[atom_num_in], atom_num_in);
			cout << file_electronic_negativity << endl;
			cin.get();
		}
	}
	fin.close();

	////最后开始读取第一电离能的相关信息
	//fin.clear();
	//fin.open(file_first_ionization_energy.c_str(), ios::in);
	//if (!fin.is_open())
	//{
	//	cout << "i can not find the file !" << file_first_ionization_energy << endl;
	//	cin.get();
	//}
	//while (fin.peek() != EOF && fin.good())
	//{
	//	fin >> atom_num_in;
	//	//cout << atom_num_in << endl;
	//	fin >> str;
	//	/*if (str[0] = '\0')
	//	{
	//		break;
	//	}*/
	//	fin >> e[atom_num_in].first_ionization_energy;
	//	//cout << e[atom_num_in].first_ionization_energy << endl;
	//	if (str[0] != atom[atom_num_in][0] || str[1] != atom[atom_num_in][1])
	//	{
	//		printf("ERROR atom name %s %s %d\n", str, atom[atom_num_in], atom_num_in);
	//		cout << file_first_ionization_energy << endl;
	//		cin.get();
	//	}
	//}
	//fin.close();
	return;
}
void read_element(element *e, string &file_element_r)
{
	int i, j, k;
	char atom_name[120][3] = { "D", "H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th", "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt", "Ds", "Rg" };

	//先初始化信息
	for (i = 0; i < 120; i++)
	{
		//strcpy(e[i].name, atom_name[i]);
		e[i].atomic_num = i;
		//e[i].num_common_val = 0;
		e[i].num_metal_radius = 0;
		//e[i].num_unusual_val = 0;
	}
	i = 0, j = 0, k = 0;
	int line_in;
	ifstream fin;
	int atom_num_in;
	double vdw_r_min_in, vdw_r_max_in;
	double cov_r;
	int num_metal_r;
	double *temp;
	char str[5];
	char str_temp[500];
	int num_common_in;
	int num_unsual_in;
	//读取共价键范德华键和金属键

	fin.open(file_element_r.c_str(), ios::in);
	if (!fin.is_open())
	{
		cout << "i can not find the file !" << file_element_r << endl;
		cin.get();
	}
	while (fin.peek() != EOF && fin.good())
	{
		fin >> atom_num_in;
		fin >> vdw_r_min_in;
		fin >> vdw_r_max_in;
		fin >> cov_r;
		fin >> num_metal_r;

		e[atom_num_in].metal_radius = new double[num_metal_r];
		double *temp = new double[num_metal_r];
		for (i = 0; i < num_metal_r; i++)
		{
			fin >> temp[i];
		}
		e[atom_num_in].atomic_num = atom_num_in;
		e[atom_num_in].vdw_radius_max = vdw_r_max_in;
		e[atom_num_in].vdw_radius_min = vdw_r_min_in;
		e[atom_num_in].cov_radius = cov_r;
		e[atom_num_in].num_metal_radius = num_metal_r;
		for (i = 0; i < num_metal_r; i++)
		{
			e[atom_num_in].metal_radius[i] = temp[i];
		}
		free(temp);
	}
	fin.close();

	return;
}
double dis(double *p1, double *p2)
{
	return sqrt(pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2) + pow(p1[2] - p2[2], 2));
}
//void new_get_style(char *style)
//{
//	ifstream fin;
//	fin.open(style, ios::in);
//	if (!fin.is_open())
//	{
//		cout << "i can not find the file:" << style << endl;
//		cin.get();
//	}
//	while (fin.good())
//	{
//		for (int i = 1; i < 112; i++)
//		{
//			for (int j = 1; j < 112; j++)
//			{
//				fin >> dist[i][j];
//			}
//		}
//	}
//	fin.close();
//	return;
//}


void full_expand_generate_graph(cell &cell_a, int **expand_matrix, int num, element *e,vector <vector<double>> &max_ionic,double**dist)
{
	//建立重载版本的构建连接关系
	int i = 0, j = 0;
	int m = 0, n = 0;
	double temp = 0.0;

	//首先尝试使用共价键连接原子
	for (i = 0; i < num; i++)
	{
		if(cell_a.my_classify[i%cell_a.num]==2)
		for (j = i + 1; j < num; j++)
		{
			if (cell_a.my_classify[j%cell_a.num] == 2)
			{
				temp = dis(cell_a.real_position[i / cell_a.num][i % cell_a.num], cell_a.real_position[j / cell_a.num][j % cell_a.num]);
				if (abs(temp) < 1e-3)
					continue;
				//检查没有共价键的情况
				if (e[cell_a.type[i%cell_a.num]].cov_radius ==-1 || e[cell_a.type[j%cell_a.num]].cov_radius ==-1)
				{
					//cout << "unknown covridus!:atomic :" << cell_a.type[i%cell_a.num] << "," << cell_a.type[j%cell_a.num] << endl;
					//cin.get();
					continue;
				}
				dist[cell_a.type[i%cell_a.num]][cell_a.type[j%cell_a.num]] = (e[cell_a.type[i%cell_a.num]].cov_radius + e[cell_a.type[j%cell_a.num]].cov_radius) / 100.0*val_radius_factor;
				dist[cell_a.type[j%cell_a.num]][cell_a.type[i%cell_a.num]] = (e[cell_a.type[i%cell_a.num]].cov_radius + e[cell_a.type[j%cell_a.num]].cov_radius) / 100.0*val_radius_factor;

				if (temp<(e[cell_a.type[i%cell_a.num]].cov_radius+ e[cell_a.type[j%cell_a.num]].cov_radius)/100.0*val_radius_factor)
				{
					//cout << e[cell_a.type[i%cell_a.num]].cov_radius + e[cell_a.type[j%cell_a.num]].cov_radius << endl;
					expand_matrix[i][j] = 1;
					expand_matrix[j][i] = 1;
				}
				else
				{
					expand_matrix[i][j] = 0;
					expand_matrix[j][i] = 0;
				}
			}
		}
	}

	//然后使用离子半径连接正负基团
	for (i = 0; i < num; i++)
	{
		if (cell_a.if_positive[i%cell_a.num] == 1)
		{
			for (j = 0; j < num; j++)
			{
				if (cell_a.if_positive[j%cell_a.num] == -1)
				{
					temp = dis(cell_a.real_position[i / cell_a.num][i % cell_a.num], cell_a.real_position[j / cell_a.num][j % cell_a.num]);
					if (abs(temp) < 1e-3)
						continue;
					//检查没有离子键的情况

					if (max_ionic[cell_a.type[i%cell_a.num]][0]<-100|| max_ionic[cell_a.type[j%cell_a.num]][1] < -100)
					{
						//cout << "unknown ionic!:atomic :" << atom_name[cell_a.type[i%cell_a.num]] << "," << atom_name[cell_a.type[j%cell_a.num]] << endl;
						//cin.get();
						continue;
					}
					double rule= (max_ionic[cell_a.type[i%cell_a.num]][0] + max_ionic[cell_a.type[j%cell_a.num]][1])*ridus_plus_factor;
					if (rule > dist[cell_a.type[i%cell_a.num]][cell_a.type[j%cell_a.num]])
					{
						//后来我们只更新距离大的style
						dist[cell_a.type[i%cell_a.num]][cell_a.type[j%cell_a.num]] = rule;
						dist[cell_a.type[j%cell_a.num]][cell_a.type[i%cell_a.num]] = rule;

					}
					
					if (temp < rule)
					{
						//cout << (max_ionic[cell_a.type[i%cell_a.num]][0] + max_ionic[cell_a.type[j%cell_a.num]][1])*ridus_plus_factor << endl;
						//cout << atom_name[cell_a.type[i%cell_a.num]] << "," << atom_name[cell_a.type[j%cell_a.num]] << endl;
						expand_matrix[i][j] = 1;
						expand_matrix[j][i] = 1;
					}				
				}
			}
		}
	}


	//然后检查金属的情况，如果金属周围是孤立的，则尝试用金属半径连接金属与金属
	for (i = 0; i < num; i++)
	{
		int iso_flag = 1;
		if (cell_a.my_classify[i%cell_a.num] == 1)
		{
			for (j = 0; j < num;j++)
			{
				if (expand_matrix[i][j] == 1)
				{
					iso_flag = 0;
					break;
				}
			}
			if (iso_flag == 1)//说明这个是孤立的金属原子
			{
				for (j = 0; j < num; j++)
				{
					if (cell_a.my_classify[j%cell_a.num] == 1)
					{
						temp = dis(cell_a.real_position[i / cell_a.num][i % cell_a.num], cell_a.real_position[j / cell_a.num][j % cell_a.num]);
						if (abs(temp) < 1e-3)
							continue;
						double rule = (e[cell_a.type[i%cell_a.num]].metal_radius[0] + e[cell_a.type[j%cell_a.num]].metal_radius[0])/100.0*metal_ridus_factor;
						
						if (rule > dist[cell_a.type[i%cell_a.num]][cell_a.type[j%cell_a.num]])
						{
							dist[cell_a.type[i%cell_a.num]][cell_a.type[j%cell_a.num]] = rule;
							dist[cell_a.type[j%cell_a.num]][cell_a.type[i%cell_a.num]] = rule;
						}	

						if (temp < rule)
						{
							/*cout << rule << temp<<endl;
							cin.get();*/
							expand_matrix[i][j] = 1;
							expand_matrix[j][i] = 1;
						}
					}
				}
			}

		}
	}
	return;
}


void full_expand_generate_graph(cell &cell_a, int **expand_matrix, int num, double ***possi, element *e) //�����ǿվ���Ͷ�Ӧ�㣬��������õľ���
{
	int i = 0, j = 0;
	int m = 0, n = 0;
	double temp = 0.0;
	for (i = 0; i < num; i++)
	{
		for (j = i + 1; j < num; j++)
		{
			//cout << "for j,this is the :" << j / cell_a.num << "matrix ," << endl;
			//cout << "the xuhao is :" << j % cell_a.num << endl;
			for (m = 0; m < cell_a.type_num; m++)
			{
				if (cell_a.type[i % cell_a.num] == cell_a.type_save[m])
				{
					break;
				}
			}
			for (n = 0; n < cell_a.type_num; n++)
			{
				if (cell_a.type[j % cell_a.num] == cell_a.type_save[n])
				{
					break;
				}
			}
			//先获得m和n控制待会更新的位置

			if (cell_a.type[i % cell_a.num] != cell_a.type[j % cell_a.num])
			{
				if (cell_a.positive[i % cell_a.num] * cell_a.positive[j % cell_a.num] == -1)
				{
					//cout << "test, " << cell_a.type[i % cell_a.num] << "," << cell_a.type[j % cell_a.num] << " different" << endl;
					//cout<<"rule:"<<ridus_plus_factor * (cell_a.ridus[i % cell_a.num] + cell_a.ridus[j % cell_a.num]) / 1.2<<endl;
					//cin.get();
					temp = dis(cell_a.real_position[i / cell_a.num][i % cell_a.num], cell_a.real_position[j / cell_a.num][j % cell_a.num]);
					if (abs(temp) < 1e-3)
						continue;
					if (temp < (ridus_plus_factor * (cell_a.ridus[i % cell_a.num][0] + cell_a.ridus[j % cell_a.num][0]) / 1.2))
					{
						if (temp > possi[m][n][0])
						{
							possi[n][m][0] = possi[m][n][0] = temp;
						}
						expand_matrix[i][j] = 1;
						expand_matrix[j][i] = 1;
					}
					else
					{
						if (temp < possi[m][n][1])
						{
							possi[n][m][1] = possi[m][n][1] = temp;
						}
						expand_matrix[i][j] = 0;
						expand_matrix[j][i] = 0;
					}
				}
				//这里我们添加上新的规则，也就是如果是负元素连键，取其中的一个正价态半径去连
				if (cell_a.positive[i % cell_a.num] + cell_a.positive[j % cell_a.num] == -2)
				{
					temp = dis(cell_a.real_position[i / cell_a.num][i % cell_a.num], cell_a.real_position[j / cell_a.num][j % cell_a.num]);
					if (abs(temp) < 1e-3)
						continue;
					if (temp < (ridus_plus_factor * (cell_a.ridus[i % cell_a.num][1] + cell_a.ridus[j % cell_a.num][0]) / 1.2) || temp < (ridus_plus_factor * (cell_a.ridus[i % cell_a.num][0] + cell_a.ridus[j % cell_a.num][1]) / 1.2))
					{
						if (temp > possi[m][n][0])
						{
							possi[n][m][0] = possi[m][n][0] = temp;
						}
						//cout << "hahah,got!" << endl;
						//cout << cell_a.ridus[i % cell_a.num][1] << endl;
						//cin.get();
						expand_matrix[i][j] = 1;
						expand_matrix[j][i] = 1;
					}
					else
					{
						if (temp < possi[m][n][1])
						{
							possi[n][m][1] = possi[m][n][1] = temp;
						}
						expand_matrix[i][j] = 0;
						expand_matrix[j][i] = 0;
					}


				}
			}
			else
			{
				//cout << "test,same type:" << cell_a.type[i % cell_a.num] << "," << cell_a.type[j % cell_a.num] << endl;
				//cout << "rule:" << val_radius_factor /100.0* (e[cell_a.type[i % cell_a.num]].cov_radius + e[cell_a.type[j % cell_a.num]].cov_radius)<<endl; 
				//cin.get();
				temp = dis(cell_a.real_position[i / cell_a.num][i % cell_a.num], cell_a.real_position[j / cell_a.num][j % cell_a.num]);
				if (abs(temp) < 1e-3)
					continue;
				if (temp < (val_radius_factor / 100.0 * (e[cell_a.type[i % cell_a.num]].cov_radius + e[cell_a.type[j % cell_a.num]].cov_radius)) && (e[cell_a.type[i % cell_a.num]].cov_radius != -1 && e[cell_a.type[j % cell_a.num]].cov_radius != -1))
				{
					if (temp > possi[m][n][0])
					{
						possi[n][m][0] = possi[m][n][0] = temp;
					}
					expand_matrix[i][j] = 1;
					expand_matrix[j][i] = 1;
				}
				else
				{
					if (temp < possi[m][n][1])
					{
						possi[n][m][1] = possi[m][n][1] = temp;
					}
					expand_matrix[i][j] = 0;
					expand_matrix[j][i] = 0;
				}
			}
		}
	}
	return;
}









void ex_find_connect(int edge_num, cell cell_a, int *visited, int **matrix, save *save_a)
{
	int i = 0, ii = 0, j = 0, k = 0, jj = 0;
	int save_flag = 1;
	int subnets = 0;
	for (k = 0; k < edge_num; k++)
	{
		if (!visited[k])
		{
			subnets++;
			ex_DFS(k, visited, cell_a, matrix, edge_num, subnets);

			//cout << "the subnets is:" << subnets << endl;
			save_a->sunbets = subnets;
			for (i = 0; i < edge_num; i++)
			{
				if (visited[i] == subnets)
				{
					save_a->save_list[subnets - 1][ii] = i;
					ii++;
				}
			}
		}
		ii = 0;
	}
	//cout << "the hanshu finished before,the subnets is:" << save_a->sunbets << endl;
	//cin.get();
	return;
}

void generate_testfile(save *save_a, cell cell_a, int edge_num) //������save*ָ�룬����ǽ����txt
{
	int i = 0, j = 0;
	FILE *out;
	out = fopen("test_output.txt", "wt");
	char a[50] = "the lian tong num is:";
	char b[20];
	sprintf(b, "%d\t", save_a->sunbets);
	strcat(a, b);
	fprintf(out, "%s\t", a);
	cout << "i got the liantong geshu is:" << save_a->sunbets << endl;
	for (i = 0; i < save_a->sunbets; i++)
	{
		for (j = 0; j < edge_num; j++)
		{
			if (save_a->save_list[i][j] != -1)
			{
				fprintf(out, "%d\t", save_a->save_list[i][j]);
			}
		}
		fprintf(out, "\n");
	}
	fclose(out);
	return;
}

void generate_outfile(cell cell_a, save *ex_pt, int edge_num, int *judge) ////��������������ǻ������ͨ����֮�󣬸����жϵĽ������txt�ļ����ڹ۲�
{
	int i, j;
	FILE *out;
	out = fopen("out_judge.txt", "w");
	char a[50] = "the sunbets num is:";
	char b[20];
	sprintf(b, "%d\t", ex_pt->sunbets);
	strcat(a, b);
	fprintf(out, "%s\n", a);
	cout << "i got the liantong geshu is:" << ex_pt->sunbets << endl;
	char c[60] = "for subnets,judge out:";
	fprintf(out, "%s\n", c);
	for (i = 0; i < ex_pt->sunbets; i++)
	{
		fprintf(out, "%d\t", judge[i]);
	}
	fprintf(out, "\n");
	for (i = 0; i < ex_pt->sunbets; i++)
	{
		for (j = 0; j < edge_num; j++)
		{
			if (ex_pt->save_list[ex_pt->sunbets - 1][j] != -1)
			{
				fprintf(out, "%d\t", ex_pt->save_list[i][j]);
			}
		}
		fprintf(out, "\n");
	}

	fclose(out);
	return;
}

void ex_DFS(int a, int *ex_visited, cell cell_a, int **expand_matrix, int num_edge, int subnets) //�������������ʵ�ִӶ���a����������������δ���ʹ����ڽӽڵ�
{
	ex_visited[a] = subnets;
	//printf("now i start the dot %d\n", a);
	int i = 0, j = 0, k = 0;
	for (j = 0; j < num_edge; j++)
	{
		if (expand_matrix[a][j] == 1 && !ex_visited[j])
		{
			ex_DFS(j, ex_visited, cell_a, expand_matrix, num_edge, subnets);
			//����д���˺������,��ʵ�ܼ򵥣����Ǽ�¼���ok��
		}
	}
}

int *judge_the_2d(cell cell_a, int *save_list, int edge_num, int *judge) //���������������������õ���ÿ����ͨ�������ж����ǲ�������Һõ�2d��ͨ����
{
	int i = 0, j = 0, k = 0, m = 0;
	int *pt = judge;
	int exit_flag = 0; //�˳���־��1��ʾ1d��2��ʾ2d��3��ʾ3d,0��ʾ���ڵ�̫���ڷ�ɢ����޷���������жϵ�����
	int *copy = new int[yanshen];
	for (int i = 0; i < yanshen; i++)
	{
		copy[i] = -1;
	}
	//cout << cell_a.num << endl;

	//�������ʵ�copy
	int temp = 0;
	for (i = 0; i < edge_num; i++)
	{
		if (save_list[i] != -1)
		{
			temp = save_list[i] % cell_a.num;
			for (k = 0; k < edge_num; k++)
			{
				if (save_list[k] == -1)
					break;
				if ((save_list[k]) % cell_a.num == temp)
				{
					copy[j] = save_list[k];
					j++;
				}
			}
			if (j > 3)
				break;
			else
				j = 0;
		}
		else
			break;
	}

	if (j < 4 || j > cengshu * cengshu)
	{
		//cout << "i can not find the enough vertex to complete the judgement" << endl;
		*pt = 0;
		*(pt + 1) = -1;
		delete[] copy;
		return pt;
	}

	if (j == cengshu * cengshu)
	{
		*pt = 2;
		*(pt + 1) = temp;
		delete[] copy;
		return pt;
	}

	//cout << "i need to judge the ppints_num is:" << j << endl;
	//cin.get();
	//debug
	/*for (i = 0; i < j; i++)
	{
		cout << copy[i] << "," << endl;
	}*/
	//�ҵ��˷��������ĸ������������

	//j���Ǹ�����ĸ���

	//�������жϹ�������
	exit_flag = 1;
	double **a = new double *[3];
	for (i = 0; i < 3; i++)
	{
		a[i] = new double[3];
	}
	a[0][0] = a[0][1] = a[0][2] = 1;

	double x = cell_a.real_position[copy[1] / cell_a.num][copy[1] % cell_a.num][0] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][0];
	double y = cell_a.real_position[copy[1] / cell_a.num][copy[1] % cell_a.num][1] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][1];
	double z = cell_a.real_position[copy[1] / cell_a.num][copy[1] % cell_a.num][2] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][2];
	a[1][0] = x;
	a[1][1] = y;
	a[1][2] = z;

	double nx = 0;
	double ny = 0;
	double nz = 0;
	for (i = 2; i < j; i++)
	{
		if (save_list[i] != -1)
		{
			nx = cell_a.real_position[copy[i] / cell_a.num][copy[i] % cell_a.num][0] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][0];
			ny = cell_a.real_position[copy[i] / cell_a.num][copy[i] % cell_a.num][1] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][1];
			nz = cell_a.real_position[copy[i] / cell_a.num][copy[i] % cell_a.num][2] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][2];

			a[2][0] = nx;
			a[2][1] = ny;
			a[2][2] = nz;
			if (abs(three_jie_chaji(a)) > 1e-3) //��һ�������ߵ�����Ϳ���
			{
				exit_flag = 0;
				break;
			}
		}
	}

	for (i = 0; i < 3; i++)
	{
		delete[] a[i];
	}
	delete[] a;
	if (exit_flag == 1)
	{
		//cout << "the 1d situation" << endl;
		*pt = 1;
		*(pt + 1) = temp;
		delete[] copy;
		return pt;
	}

	//Ȼ�����Ƿ��ĵ㹲�����
	exit_flag = 2;
	double **b = new double *[4];
	for (i = 0; i < 4; i++)
	{
		b[i] = new double[3];
	}
	int number = j; //�ȰѸ���������
	int *flag = new int[number];
	for (i = 0; i < number; i++)
	{
		flag[i] = 0;
	}

	int jj = 0;
	for (i = 0; i < number; i++)
	{
		flag[i] = 1;
		for (j = i + 1; j < number; j++)
		{
			flag[j] = 1;
			for (k = j + 1; k < number; k++)
			{
				flag[k] = 1;
				for (m = k + 1; m < number; m++)
				{
					flag[m] = 1;

					for (int mm = 0; mm < number; mm++) //�����ͻ�õ�4��������
					{
						if (flag[mm] == 1)
						{
							for (int ii = 0; ii < 3; ii++)
							{
								b[jj][ii] = cell_a.real_position[copy[mm] / cell_a.num][copy[mm] % cell_a.num][ii];
							}
							jj++;
							if (jj == 4)
								break;
						}
					}
					jj = 0;
					//������д�жϹ���,���淵��1
					//ֻҪ���ַǹ������������3d���
					if (inone_face(b) == 0)
					{
						exit_flag = 3;
						for (int i = 0; i < 4; i++)
						{
							delete[] b[i];
						}
						delete[] b;
						delete[] copy;
						delete[] flag;
						*pt = 3;
						*(pt + 1) = temp;
						return pt;
					}

					flag[m] = 0;
				}
				flag[k] = 0;
			}

			flag[j] = 0;
		}
		flag[i] = 0;
	}

	*pt = 2;
	//���������Ǿ͵õ��������2d�����;
	*(pt + 1) = temp;
	delete[] copy;
	delete[] flag;
	return pt;
}

int generate_atom(save *ex_pt, int *judge, int edge_num, cell cell_a, string name, int *list)
{

	//这个的作用是根据得到的连通信息生成atom文件,输入是存储的连通分量，输出是atom文件

	int i = 0, j = 0;
	int k = 0;
	int exit_flag = 0;
	int orig = judge[1];
	int metal_count = 0;
	int metal_same_falg = 0;
	//cout << "这里的对应原始点是：" << orig << endl;
	string file[2];
	int num = cell_a.num;
	file[0] = "Lattice vector";
	file[1] = "Position";
	//首先开始生成abc的三个基矢分量
	//从原型序号中找到这样的点以及这样的距离

	double **letice = new double *[3];
	for (i = 0; i < 3; i++)
	{
		letice[i] = new double[3]; //建立letice数组用来储存
	}
	double **letice_r = new double *[3];
	for (i = 0; i < 3; i++)
	{
		letice_r[i] = new double[3];
	}
	for (i = 0; i < edge_num; i++)
	{
		if (list[i] == -1)
			break;
		if (list[i] % cell_a.num == orig)
		{
			j++;
		}
	}
	int copy_num = j;
	int *copy = new int[j];
	for (i = 0; i < edge_num; i++)
	{
		if (list[i] == -1)
			break;
		if (list[i] % cell_a.num == orig)
		{
			copy[k++] = list[i];
		}
	}

	//有j个这样的复制体,并且都找到了，并且也判断了哪些原子需要放进去
	int *atom_flag_a = new int[cell_a.num];
	//这里的atom_falg_a针对的是第一种情况的二维材料
	for (i = 0; i < cell_a.num; i++)
	{
		atom_flag_a[i] = 0;
	}
	k = 0;
	for (i = 0; i < edge_num; i++)
	{
		if (list[i] == -1)
			break;
		for (j = 0; j < cell_a.num; j++)
		{
			if (list[i] % cell_a.num == j && atom_flag_a[j] == 0)
			{
				atom_flag_a[j] = 1;
				k++;
				break;
			}
		}
	}
	int atom_num_write = k;
	double **fenshu = new double *[atom_num_write];
	for (i = 0; i < k; i++)
	{
		fenshu[i] = new double[3];
	}
	double **fenshu_plus = new double *[edge_num];
	for (i = 0; i < edge_num; i++)
	{
		fenshu_plus[i] = new double[3];
	}
	int atom_plus = 0;

	// 然后根据两种情况进行分类
	if (copy_num == cengshu * cengshu)
	{

		int m = 0;
		for (i = 0; i < 3; i++)
		{
			letice[0][i] = cell_a.real_position[copy[1] / cell_a.num][copy[1] % cell_a.num][i] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][i];
			letice[1][i] = cell_a.real_position[copy[copy_num / cengshu] / cell_a.num][copy[copy_num / cengshu] % cell_a.num][i] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][i];
		}
		letice[2][0] = letice[0][1] * letice[1][2] - letice[0][2] * letice[1][1];
		letice[2][1] = letice[0][2] * letice[1][0] - letice[0][0] * letice[1][2];
		letice[2][2] = letice[0][0] * letice[1][1] - letice[0][1] * letice[1][0];
		double plus = get_vacumlayer_thick(list, edge_num, orig, cell_a, letice, copy);

		//然后将c的模长度拉长，保证真空层是严格的25的距离
		double pingfanghe = pow(letice[2][0], 2) + pow(letice[2][1], 2) + pow(letice[2][2], 2);
		double a = pow((pow(20 + 2 * plus, 2) / pingfanghe), 0.5);
		for (i = 0; i < 3; i++)
		{
			letice[2][i] = a * letice[2][i];
		}
		/*double yuxian = (letice[0][0] * letice[1][0] + letice[0][1] * letice[1][1] + letice[0][2] * letice[1][2]) / (pow(letice[0][0] * letice[0][0] + letice[0][1] * letice[0][1] + letice[0][2] * letice[0][2], 0.5)*pow(letice[1][0] * letice[1][0] + letice[1][1] * letice[1][1] + letice[1][2] * letice[1][2], 0.5));
		double theta = acos(yuxian);
		double mo[3];
		for (i = 0; i < 3; i++)
		{
			mo[i] = pow(letice[i][0] * letice[i][0] + letice[i][1] * letice[i][1] + letice[i][2] * letice[i][2], 0.5);
		}

		for (i = 0; i < 3; i++)
		{
			if (i == 0)
			{
				letice[i][0] = mo[0];
				letice[i][1] = 0;
				letice[i][2] = 0;
			}
			if (i == 1)
			{
				letice[i][0] = mo[1] * cos(theta);
				letice[i][1] = mo[1] * sin(theta);
				letice[i][2] = 0;
			}
			if (i == 2)
			{
				letice[i][0] = letice[i][1] = 0;
				letice[i][2] = mo[2];
			}
		}*/
		if (reverse_matrix(letice, 3, letice_r) == 0)
			return 0; //建立了逆矩阵
		//这里我们需要做一步，就是将产生的矩阵做成是尽量0多的

		//到这里完成了基矢的获取，下面是确定需要放哪个原子，以及新原子的位置是哪里
		//这里需要注意，我们首先需要知道偏移了多少，然后才能根据偏移方向针对性的写出原子真实坐标

		//首先进行原子平移,选出c最小的原子进行平移
		j = 0;

		//改变之前的做法，这里让即使横平竖直的情况，坐标变换也是基于筛选
		int metal_allflag = 99;
		for (i = 0; i < edge_num; i++)
		{
			if (list[i] == -1)
				break;
			for (m = 0; m < 3; m++)
			{
				fenshu_plus[i][m] = (cell_a.real_position[list[i] / cell_a.num][list[i] % cell_a.num][0] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][0]) * letice_r[0][m] + (cell_a.real_position[list[i] / cell_a.num][list[i] % cell_a.num][1] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][1]) * letice_r[1][m] + (cell_a.real_position[list[i] / cell_a.num][list[i] % cell_a.num][2] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][2]) * letice_r[2][m];
			}
			if (fenshu_plus[i][0] >= 1 - 1e-3 || fenshu_plus[i][0] < -1e-3 || fenshu_plus[i][1] >= 1 - 1e-3 || fenshu_plus[i][1] < -1e-3)
			{
				fenshu_plus[i][0] = -100;
			}
			if (fenshu_plus[i][0] != -100)
			{
			/*	if (metal_allflag == 99)
				{
					for (metal_count = 0; metal_count < 88; metal_count++)
					{
						if (cell_a.type[list[i] % cell_a.num] < meatal_xuhao[metal_count])
						{
							metal_same_falg = 1;
							metal_allflag = 0;
							break;
						}
						if (meatal_xuhao[metal_count] == cell_a.type[list[i] % cell_a.num])
							break;
					}
				}*/
				atom_plus++;
			}
		}
		//if (metal_allflag == 99)
		//{
		//	//cout<<"2d metal situation"<<endl;
		//	//cin.get();
		//	return -5;
		//}
		//反变换之后开始进行c方向偏移
		double c_pingyi = 100;
		i = 0, j = 0;
		while (i < edge_num)
		{
			if (list[i] == -1)
				break;
			if (fenshu_plus[i][0] != -100)
			{
				if (fenshu_plus[i][2] < c_pingyi)
				{
					c_pingyi = fenshu_plus[i][2];
				}
			}
			i++;
		}

		for (i = 0; i < edge_num; i++)
		{
			if (list[i] == -1)
				break;
			if (fenshu_plus[i][0] != -100)
			{
				fenshu_plus[i][2] = fenshu_plus[i][2] - c_pingyi + 0.02;
			}
		}
	}

	else
	{
		//cout << "开始进入偏移的状态了哦，哈哈哈哈哈" << endl;
		//cout << "the file is:" << wenjian << endl;
		//   cin.get();

		//从这里要确认怎么生成基矢,这里生成基矢的原则是面积产生最小且最接近90°
		double **chaji_save = new double *[3];
		for (i = 0; i < 3; i++)
		{
			chaji_save[i] = new double[3];
		}
		for (i = 0; i < 3; i++)
		{
			chaji_save[1][i] = cell_a.real_position[copy[1] / cell_a.num][copy[1] % cell_a.num][i] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][i];
		}

		double jvli = 100;
		double temp_jvli;
		int *mianji_small = new int[cengshu * cengshu];
		for (i = 0; i < cengshu * cengshu; i++)
		{
			mianji_small[i] = -1;
		}

		for (i = 2; i < copy_num; i++)
		{
			for (j = 0; j < 3; j++)
			{
				chaji_save[2][j] = cell_a.real_position[copy[i] / cell_a.num][copy[i] % cell_a.num][j] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][j];
			}
			temp_jvli = three_jie_chaji(chaji_save);
			if (abs(temp_jvli) > 1e-6 && temp_jvli < jvli)
			{
				jvli = temp_jvli;
			}
		}

		for (i = 2, k = 0; i < copy_num; i++)
		{
			for (j = 0; j < 3; j++)
			{
				chaji_save[2][j] = cell_a.real_position[copy[i] / cell_a.num][copy[i] % cell_a.num][j] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][j];
			}
			temp_jvli = three_jie_chaji(chaji_save);
			if (abs(temp_jvli) > 1e-6 && abs(temp_jvli - jvli) < 1e-1)
			{
				mianji_small[k] = copy[i];
				k++;
			}
		}
		//先找到了最小的面积是多少
		//然后看最接近90°的情况
		double angle_cha = 100;
		double angle_cha_temp = 0;
		double **vector = new double *[2];
		for (i = 0; i < 2; i++)
		{
			vector[i] = new double[3];
		}
		for (i = 0; i < 3; i++)
		{
			vector[0][i] = cell_a.real_position[copy[1] / cell_a.num][copy[1] % cell_a.num][i] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][i];
		}

		int m = 0;
		for (i = 0; i < k; i++)
		{
			if (mianji_small[i] == -1)
				break;
			for (j = 0; j < 3; j++)
			{
				vector[1][j] = cell_a.real_position[mianji_small[i] / cell_a.num][mianji_small[i] % cell_a.num][j] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][j];
			}
			angle_cha_temp = abs(vector_angle(vector) - 90);
			if (angle_cha_temp < angle_cha)
			{
				angle_cha = angle_cha_temp;
				m = mianji_small[i];
			}
		}
		for (i = 0; i < 3; i++)
		{
			letice[0][i] = cell_a.real_position[copy[1] / cell_a.num][copy[1] % cell_a.num][i] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][i];
			letice[1][i] = cell_a.real_position[m / cell_a.num][m % cell_a.num][i] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][i];
		}
		letice[2][0] = letice[0][1] * letice[1][2] - letice[0][2] * letice[1][1];
		letice[2][1] = letice[0][2] * letice[1][0] - letice[0][0] * letice[1][2];
		letice[2][2] = letice[0][0] * letice[1][1] - letice[0][1] * letice[1][0];
		double plus = get_vacumlayer_thick(list, edge_num, orig, cell_a, letice, copy);

		//然后将c的模长度拉长，保证真空层是严格的25的距离
		double pingfanghe = pow(letice[2][0], 2) + pow(letice[2][1], 2) + pow(letice[2][2], 2);
		double a = pow((pow(20 + 2 * plus, 2) / pingfanghe), 0.5);
		for (i = 0; i < 3; i++)
		{
			letice[2][i] = a * letice[2][i];
		}
		/*for (i = 0; i < 3; i++)
		{
			cout<<letice[2][i]<<endl;
		}
*/
/*double yuxian = (letice[0][0] * letice[1][0] + letice[0][1] * letice[1][1] + letice[0][2] * letice[1][2]) / (pow(letice[0][0] * letice[0][0] + letice[0][1] * letice[0][1] + letice[0][2] * letice[0][2], 0.5)*pow(letice[1][0] * letice[1][0] + letice[1][1] * letice[1][1] + letice[1][2] * letice[1][2], 0.5));
double theta = acos(yuxian);
double mo[3];
for (i = 0; i < 3; i++)
{
	mo[i] = pow(letice[i][0] * letice[i][0] + letice[i][1] * letice[i][1] + letice[i][2] * letice[i][2], 0.5);
}

for (i = 0; i < 3; i++)
{
	if (i == 0)
	{
		letice[i][0] = mo[0];
		letice[i][1] = 0;
		letice[i][2] = 0;
	}
	if (i == 1)
	{
		letice[i][0] = mo[1] * cos(theta);
		letice[i][1] = mo[1] * sin(theta);
		letice[i][2] = 0;
	}
	if (i == 2)
	{
		letice[i][0] = letice[i][1] = 0;
		letice[i][2] = mo[2];
	}
}*/
		if (reverse_matrix(letice, 3, letice_r) == 0)
			return 0; //建立了逆矩阵

		//现在遇到了问题，就是实际上放进去的原子个数应该是多余一个晶胞的，或者是不止一个晶胞个数，而应该要把包括的都放进去
		//这里我们这么做，先全部放进去。然后筛选
		//这里进一步筛选出纯金属的2D材料
		int metal_allflag = 99;
		for (i = 0; i < edge_num; i++)
		{
			if (list[i] == -1)
				break;
			for (m = 0; m < 3; m++)
			{
				fenshu_plus[i][m] = (cell_a.real_position[list[i] / cell_a.num][list[i] % cell_a.num][0] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][0]) * letice_r[0][m] + (cell_a.real_position[list[i] / cell_a.num][list[i] % cell_a.num][1] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][1]) * letice_r[1][m] + (cell_a.real_position[list[i] / cell_a.num][list[i] % cell_a.num][2] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][2]) * letice_r[2][m];
			}
			if (fenshu_plus[i][0] >= 1 - 1e-3 || fenshu_plus[i][0] < -1e-3 || fenshu_plus[i][1] >= 1 - 1e-3 || fenshu_plus[i][1] < -1e-3)
			{
				fenshu_plus[i][0] = -100;
			}
			if (fenshu_plus[i][0] != -100)
			{
				//说明i被选中了，看看它
				/*if (metal_allflag == 99)
				{
					for (metal_count = 0; metal_count < 88; metal_count++)
					{
						if (cell_a.type[list[i] % cell_a.num] < meatal_xuhao[metal_count])
						{
							metal_same_falg = 1;
							metal_allflag = 0;
							break;
						}
						if (meatal_xuhao[metal_count] == cell_a.type[list[i] % cell_a.num])
							break;
					}
				}*/
				atom_plus++;
			}
		}
		//if (metal_allflag == 99)
		//{
		//	//cout << "2d metal situation" << endl;
		//	//cin.get();
		//	return -5;
		//}

		//反变换之后开始进行c方向偏移
		double c_pingyi = 100;
		i = 0, j = 0;
		while (i < edge_num)
		{
			if (list[i] == -1)
				break;
			if (fenshu_plus[i][0] != -100)
			{
				if (fenshu_plus[i][2] < c_pingyi)
				{
					c_pingyi = fenshu_plus[i][2];
				}
			}
			i++;
		}

		for (i = 0; i < edge_num; i++)
		{
			if (list[i] == -1)
				break;
			if (fenshu_plus[i][0] != -100)
			{
				fenshu_plus[i][2] = fenshu_plus[i][2] - c_pingyi + 0.02;
			}
		}

		for (i = 0; i < 3; i++)
		{
			delete[] chaji_save[i];
		}
		delete[] chaji_save;
		delete[] mianji_small;
		for (i = 0; i < 2; i++)
		{
			delete[] vector[i];
		}
		delete[] vector;
		//cin.get();
	}

	//然后我生成了新的坐标，开始写文件吧
	FILE *out = fopen(name.c_str(), "wt");
	char atom_head[20] = "atoms";

	if (copy_num == cengshu * cengshu)
	{
		fprintf(out, "%d\n", atom_plus);
	}
	else
	{
		fprintf(out, "%d\n", atom_plus);
	}

	//fprintf(out, "%s\n", atom_head);
	fprintf(out, "%s\n", file[0].c_str());
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			fprintf(out, "\t%.9lf\t", letice[i][j]);
		}
		fprintf(out, "\n");
	}
	fprintf(out, "%s\n", file[1].c_str());
	j = 0;

	if (copy_num == cengshu * cengshu)
	{
		for (i = 0; i < edge_num; i++)
		{
			if (list[i] == -1)
				break;
			int mm = 1;
			if (fenshu_plus[i][0] != -100)
			{
				fprintf(out, "  %d\t", cell_a.type[list[i] % cell_a.num]);
				for (int m = 0; m < 3; m++)
				{
					fprintf(out, "%.9lf\t", fenshu_plus[i][m]);
				}
				for (int m = 0; m < 3; m++)
				{
					fprintf(out, "%d\t", mm);
				}
				fprintf(out, "\n");
			}
		}
		fclose(out);
	}
	else
	{
		for (i = 0; i < edge_num; i++)
		{
			if (list[i] == -1)
				break;
			int mm = 1;
			if (fenshu_plus[i][0] != -100)
			{
				fprintf(out, "  %d\t", cell_a.type[list[i] % cell_a.num]);
				for (int m = 0; m < 3; m++)
				{
					fprintf(out, "%.9lf\t", fenshu_plus[i][m]);
				}
				for (int m = 0; m < 3; m++)
				{
					fprintf(out, "%d\t", mm);
				}
				fprintf(out, "\n");
			}
		}
		fclose(out);
	}

	for (i = 0; i < atom_num_write; i++)
	{
		delete[] fenshu[i];
	}
	delete[] fenshu;
	for (i = 0; i < 3; i++)
	{
		delete[] letice_r[i];
	}
	delete[] letice_r;
	delete[] copy;
	for (i = 0; i < 3; i++)
	{
		delete[] letice[i];
	}
	delete[] letice;
	delete[] atom_flag_a;
	for (i = 0; i < edge_num; i++)
	{
		delete[] fenshu_plus[i];
	}
	delete[] fenshu_plus;
	return 1;
}

double get_vacumlayer_thick(int *list, int edge_num, int orig, cell cell_a, double **letice, int *copy)
{
	int i = 0, j = 0;
	int *dis_flag = new int[edge_num];
	for (int i = 0; i < edge_num; i++)
	{
		dis_flag[i] = 0;
	}
	//这个用来标记这个连通分量中哪些需要进行求距离的运算

	int *atom_flag = new int[cell_a.num];
	for (int i = 0; i < cell_a.num; i++)
	{
		atom_flag[i] = 0;
	} //标记哪些原子用到了
	for (int i = 0; i < edge_num; i++)
	{
		if (list[i] == -1)
			break;
		for (int j = 0; j < cell_a.num; j++)
		{
			if ((atom_flag[j] != 1) && (list[i] % cell_a.num == j))
			{
				atom_flag[j] = 1;
				break;
			}
		}
	}
	for (i = 0; i < edge_num; i++)
	{
		if (list[i] == -1)
			break;
		else
		{
			if (list[i] % cell_a.num != orig && atom_flag[list[i] % cell_a.num] == 1)
			{
				dis_flag[i] = 1;
				atom_flag[list[i] % cell_a.num]++;
			}
		}
	}
	//到这里完成了该计算哪些原子的点面距离了

	double plus = 0;
	double temp_dis = 0;
	//其实现在a和b就是两个向量，c就是法向量
	double xiangliang[2][3];
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			xiangliang[i][j] = letice[i][j];
		}
	}
	for (i = 0; i < edge_num; i++)
	{
		if (list[i] != -1 && dis_flag[i] == 1)
		{
			//从连通分量中找出点到面的距离
			temp_dis = face_point_dis(xiangliang, cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num], cell_a.real_position[list[i] / cell_a.num][list[i] % cell_a.num]);
			if (temp_dis > plus)
			{
				plus = temp_dis;
			}
		}
	}
	delete[] dis_flag;
	delete[] atom_flag;
	return plus;
}
inline double three_jie_chaji(double **a)
{
	int i = 0, j = 0;
	double temp = 0;
	temp = pow(pow((a[1][1] * a[2][2] - a[1][2] * a[2][1]), 2) + pow((a[1][2] * a[2][0] - a[1][0] * a[2][2]), 2) + pow((a[1][0] * a[2][1] - a[1][1] * a[2][0]), 2), 0.5);
	return temp;
}

inline int inone_face(double **a) //���淵��1�������淵��0
{
	int i = 0, j = 0;
	double x1, x2, x3, y1, y2, y3, z1, z2, z3;
	x1 = a[0][0] - a[3][0];
	x2 = a[1][0] - a[3][0];
	x3 = a[2][0] - a[3][0];

	y1 = a[0][1] - a[3][1];
	y2 = a[1][1] - a[3][1];
	y3 = a[2][1] - a[3][1];

	z1 = a[0][2] - a[3][2];
	z2 = a[1][2] - a[3][2];
	z3 = a[2][2] - a[3][2];
	double k = 0;
	k = (x1 * y2 * z3) + (x2 * y3 * z1) + (x3 * y1 * z2) - (x3 * y2 * z1) - (y3 * z2 * x1) - (z3 * x2 * y1);

	if (-1e-1 < k && k < 1e-1)
		return 1;
	else
		return 0;
}

double det(double **a, int num)
{
	int i = 0, j = 0, k = 0;
	if (num == 1)
	{
		return a[0][0];
	}
	double ans = 0;
	double **temp_det = new double *[num];
	for (i = 0; i < num; i++)
	{
		temp_det[i] = new double[num];
	}

	for (i = 0; i < num; i++)
	{
		for (j = 0; j < num - 1; j++)
		{
			for (k = 0; k < num - 1; k++)
			{
				temp_det[j][k] = a[j + 1][(k >= i) ? k + 1 : k];
			}
		}
		double t = det(temp_det, num - 1);

		if (i % 2 == 0)
		{
			ans = ans + a[0][i] * t;
		}
		else
		{
			ans = ans - a[0][i] * t;
		}
	}
	return ans;
}

//����һ����������ĺ�����������ԭ��������Ƕ�Ӧλ�ñ�����ʽ���İ������
void getastar(double **a, int num, double **b)
{
	int i = 0, j = 0, k = 0, m = 0;
	if (num == 1)
	{
		b[0][0] = 1;
		return;
	}
	double **temp = new double *[num];
	for (i = 0; i < num; i++)
	{
		temp[i] = new double[num];
	}
	for (i = 0; i < num; i++)
	{
		for (j = 0; j < num; j++)
		{
			for (k = 0; k < num - 1; k++)
			{
				for (m = 0; m < num - 1; m++)
				{
					temp[k][m] = a[(k >= i) ? k + 1 : k][(m >= j) ? m + 1 : m];
				}
			}

			b[j][i] = det(temp, num - 1);
			if ((i + j) % 2 != 0)
			{
				b[j][i] = -b[j][i];
			}
		}
	}
	for (i = 0; i < num; i++)
	{
		delete[] temp[i];
	}
	delete[] temp;
	return;
}

int reverse_matrix(double **a, int num, double **b)
{
	int i = 0, j = 0;
	double **temp = new double *[num];
	for (i = 0; i < num; i++)
	{
		temp[i] = new double[num];
	}
	getastar(a, num, temp);
	double value = det(a, num);
	if (abs(value) < 1e-4)
	{
		cout << "can not reverse!please check!" << endl;
		return 0;
	}
	for (i = 0; i < num; i++)
	{
		for (j = 0; j < num; j++)
		{
			b[i][j] = temp[i][j] / value;
		}
	}
	return 1;
}

string int2string(int i)
{

	stringstream ss;
	ss << i;

	return ss.str();
}

inline double face_point_dis(double letice[2][3], double in_face_point[3], double point[3])
{
	int i = 0, j = 0;
	double fa[3] = { 0 };
	fa[0] = letice[0][1] * letice[1][2] - letice[0][2] * letice[1][1];
	fa[1] = letice[0][2] * letice[1][0] - letice[0][0] * letice[1][2];
	fa[2] = letice[0][0] * letice[1][1] - letice[0][1] * letice[1][0];
	double qp[3] = { 0 };
	for (i = 0; i < 3; i++)
	{
		qp[i] = in_face_point[i] - point[i];
	}
	double fenzi = qp[0] * fa[0] + qp[1] * fa[1] + qp[2] * fa[2];
	double dis = 0;
	dis = abs(fenzi / pow(pow(fa[0], 2) + pow(fa[1], 2) + pow(fa[2], 2), 0.5));
	return dis;
}

inline double vector_angle(double **a)
{
	//用来求两个向量的夹角，结果以角度制返回

	double a_mu = pow(pow(a[0][0], 2) + pow(a[0][1], 2) + pow(a[0][2], 2), 0.5);
	double b_mu = pow(pow(a[1][0], 2) + pow(a[1][1], 2) + pow(a[1][2], 2), 0.5);
	double diancheng = a[0][0] * a[1][0] + a[0][1] * a[1][1] + a[0][2] * a[1][2];
	double jungel_orig = acos(diancheng / (a_mu * b_mu)) * 180 / pi;
	//cout << "the judge angle is:" << jungel_orig << endl;
	return jungel_orig;
}

void buble_plus(double *a, int *b, int num)
{
	int i = 0, j = 0;
	int flag = 0;
	for (i = 0; i < num; i++)
	{
		b[i] = i;
	}
	double temp;
	int temp_xuhao;
	for (i = num - 1; i >= 0; i--)
	{
		flag = 0;
		for (j = 0; j < i; j++)
		{
			if (a[j] > a[j + 1]) //如果是这样的话需要交换
			{
				flag = 1;
				temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;

				temp_xuhao = b[j];
				b[j] = b[j + 1];
				b[j + 1] = temp_xuhao;
			}
		}
		if (flag == 0)
			break;
	}
	return;
}


bool if_valence_conncet(cell cell_a, element* e)
{
	//输入一个结构判断结构是否存在共价连接的情况
	int i = 0, j = 0;
	double dise = 0;
	for (i = 0; i < cell_a.num; i++)
	{
		if (cell_a.my_classify[i] == 2)
		{
			for (j = 0; j < yanshen*cell_a.num; j++)
			{
				if (cell_a.my_classify[j%cell_a.num] == 2)
				{
					//此时i是主族元素，j是周边的主族元素
					dise = dis(cell_a.real_position[13][i], cell_a.real_position[j / cell_a.num][j%cell_a.num]);
					if (abs(dise) > 1e-3 && dise<(e[cell_a.type[i]].cov_radius+e[cell_a.type[j%cell_a.num]].cov_radius)/100.0*val_radius_factor)
					{
						//说明存在共价元素
						//标记退出
						return true;
					}
				}
			}
		}
	}
	return false;
}


int *judge_the_1d(cell cell_a, int *save_list, int edge_num, int *judge) //���������������������õ���ÿ����ͨ�������ж����ǲ�������Һõ�2d��ͨ����
{
	int i = 0, j = 0, k = 0, m = 0;
	int *pt = judge;
	int exit_flag = 0; //�˳���־��1��ʾ1d��2��ʾ2d��3��ʾ3d,0��ʾ���ڵ�̫���ڷ�ɢ����޷���������жϵ�����
	int *copy = new int[yanshen];
	for (int i = 0; i < yanshen; i++)
	{
		copy[i] = -1;
	}
	//�������ʵ�copy
	int temp = 0;
	for (i = 0; i < edge_num; i++)
	{
		if (save_list[i] != -1)
		{
			temp = save_list[i] % cell_a.num;
			for (k = 0; k < edge_num; k++)
			{
				if (save_list[k] == -1)
					break;
				if ((save_list[k]) % cell_a.num == temp)
				{
					copy[j] = save_list[k];
					j++;
				}
			}
			//cout << j << endl;
			if (j > (cengshu-1))
				break;
			else
				j = 0;
		}
		else
			break;
	}

	//cout << j << endl;
	if (j !=cengshu)
	{
		//cout << "i can not find the enough vertex to complete the judgement" << endl;
		
		*pt = 0;
		*(pt + 1) = -1;
		delete[] copy;
		return pt;
	}

	
	exit_flag = 1;
	double **a = new double *[3];
	for (i = 0; i < 3; i++)
	{
		a[i] = new double[3];
	}
	a[0][0] = a[0][1] = a[0][2] = 1;

	double x = cell_a.real_position[copy[1] / cell_a.num][copy[1] % cell_a.num][0] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][0];
	double y = cell_a.real_position[copy[1] / cell_a.num][copy[1] % cell_a.num][1] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][1];
	double z = cell_a.real_position[copy[1] / cell_a.num][copy[1] % cell_a.num][2] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][2];
	a[1][0] = x;
	a[1][1] = y;
	a[1][2] = z;

	double nx = 0;
	double ny = 0;
	double nz = 0;
	for (i = 2; i < j; i++)
	{
		if (save_list[i] != -1)
		{
			nx = cell_a.real_position[copy[i] / cell_a.num][copy[i] % cell_a.num][0] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][0];
			ny = cell_a.real_position[copy[i] / cell_a.num][copy[i] % cell_a.num][1] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][1];
			nz = cell_a.real_position[copy[i] / cell_a.num][copy[i] % cell_a.num][2] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][2];

			a[2][0] = nx;
			a[2][1] = ny;
			a[2][2] = nz;
			if (abs(three_jie_chaji(a)) > 1e-3) //��һ�������ߵ�����Ϳ���
			{
				exit_flag = 0;
				break;
			}
		}
	}

	for (i = 0; i < 3; i++)
	{
		delete[] a[i];
	}
	delete[] a;
	if (exit_flag == 1)
	{
		//cout << "the 1d situation" << endl;
		*pt = 1;
		*(pt + 1) = temp;
		delete[] copy;
		return pt;
	}
	else
	{
		*pt = 5; //我们这里只需要查找到1D的情况
		*(pt + 1) = temp;
		delete[] copy;
		return pt;
	}	
	return pt;
}

int generate_atom_1d(save *ex_pt, int *judge, int edge_num, cell cell_a, string name, int *list,int** expand_graph)
{

	//这个的作用是根据得到的连通信息生成atom文件,输入是存储的连通分量，输出是atom文件

	int i = 0, j = 0;
	int k = 0;
	int exit_flag = 0;
	int orig = judge[1];
	//cout << "这里的对应原始点是：" << orig << endl;
	string file[2];
	int num = cell_a.num;
	file[0] = "Lattice vector";
	file[1] = "Position";
	//首先开始生成abc的三个基矢分量
	//从原型序号中找到这样的点以及这样的距离

	double **letice = new double *[3];
	for (i = 0; i < 3; i++)
	{
		letice[i] = new double[3]; //建立letice数组用来储存
	}
	double **letice_r = new double *[3];
	for (i = 0; i < 3; i++)
	{
		letice_r[i] = new double[3];
	}
	for (i = 0; i < edge_num; i++)
	{
		if (list[i] == -1)
			break;
		if (list[i] % cell_a.num == orig)
		{
			j++;
		}
	}
	int copy_num = j;
	int *copy = new int[j];
	for (i = 0; i < edge_num; i++)
	{
		if (list[i] == -1)
			break;
		if (list[i] % cell_a.num == orig)
		{
			copy[k++] = list[i];
		}
	}

	//有j个这样的复制体,并且都找到了，并且也判断了哪些原子需要放进去
	int *atom_flag_a = new int[cell_a.num];
	//这里的atom_falg_a针对的是第一种情况的二维材料
	for (i = 0; i < cell_a.num; i++)
	{
		atom_flag_a[i] = 0;
	}
	k = 0;
	for (i = 0; i < edge_num; i++)
	{
		if (list[i] == -1)
			break;
		for (j = 0; j < cell_a.num; j++)
		{
			if (list[i] % cell_a.num == j && atom_flag_a[j] == 0)
			{
				atom_flag_a[j] = 1;
				k++;
				break;
			}
		}
	}
	int atom_num_write = k;
	double **fenshu = new double *[atom_num_write];
	for (i = 0; i < k; i++)
	{
		fenshu[i] = new double[3];
	}
	double **fenshu_plus = new double *[edge_num];
	for (i = 0; i < edge_num; i++)
	{
		fenshu_plus[i] = new double[3];
	}
	int atom_plus = 0;

	// 然后根据两种情况进行分类

	//相对于二维情况，一维情况我们只需要关注a方向的基矢，b可以自己创造，c也是基于此创造出来
	//明确原子放进来的机制
	int m = 0;
	for (i = 0; i < 3; i++)
	{
		letice[0][i] = cell_a.real_position[copy[1] / cell_a.num][copy[1] % cell_a.num][i] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][i];
		//letice[1][i] = cell_a.real_position[copy[copy_num / cengshu] / cell_a.num][copy[copy_num / cengshu] % cell_a.num][i] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][i];
	}
	j = 0;
	//然后创造出b的基矢
	//这里b的基矢是指与第一个点相连之中，最垂直，距离最远的那个,从距离最远的依次看角度
	int connect_num = 0;
	for (i = 0; i < edge_num; i++)
	{
		if (i != copy[0] && expand_graph[copy[0]][i] == 1)
		{
			connect_num++;
		}
	}
	struct three_double* my_double = new struct three_double[connect_num];


	for (i = 0; i < edge_num; i++)
	{
		if (i != copy[0] && expand_graph[copy[0]][i] == 1)
		{
			my_double[j].distnace = dis(cell_a.real_position[copy[0]/cell_a.num][copy[0]%cell_a.num], cell_a.real_position[i/cell_a.num][i%cell_a.num]);
			my_double[j].xuhao = i;
			j++;
		}
	}
	/*for (i = 0; i < connect_num; i++)
	{
		cout << my_double[i].distnace << "," << my_double[i].xuhao << endl;
	}*/
	qsort((void*)&my_double[0],connect_num,sizeof(struct three_double),Comp);
	/*for (i = 0; i < connect_num; i++)
	{
		cout << my_double[i].distnace << "," << my_double[i].xuhao << endl;
	}*/
	//按照距离从大到小排序完了
	//下面开始查看他们的角度
	double distance_cha = 0;
	double **vector = new double*[2];
	for (i = 0; i < 2; i++)
	{
		vector[i] = new double[3];
	}
	for (i = 0; i < 3; i++)
	{
		vector[0][i] = letice[0][i];
	}
	//这里预防一个情况，如果找不到角度合适的怎么办？
	for (i = 0; i < connect_num; i++)
	{
		for (j = 0; j < 3; j++)
		{
			vector[1][j] = cell_a.real_position[my_double[i].xuhao / cell_a.num][my_double[i].xuhao % cell_a.num][j] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][j];
			for (j = 0; j < 3; j++)
			{
				letice[1][j] = 3 * (cell_a.real_position[my_double[i].xuhao / cell_a.num][my_double[i].xuhao % cell_a.num][j] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][j]);
			}
		}
		double angle = abs(vector_angle(vector));
		if (angle > double(90-oneD_angel_rule) &&angle<double(90+oneD_angel_rule) )
		{
			distance_cha = abs(my_double[i].distnace - my_double[0].distnace);
			for (j = 0; j < 3; j++)
			{
				letice[1][j] = 3*(cell_a.real_position[my_double[i].xuhao / cell_a.num][my_double[i].xuhao % cell_a.num][j] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][j]);
			}
			break;
		}

	}
	delete[]my_double;
	for (i = 0; i < 2; i++)
	{
		delete[]vector[i];
	}
	delete[]vector;

	////控制b和c的长度	
	/*double pingfanghe = pow(letice[1][0], 2) + pow(letice[1][1], 2) + pow(letice[1][2], 2);
	double a = pow((pow(5 + distance_cha, 2) / pingfanghe), 0.5);
	for (i = 0; i < 3; i++)
	{
		letice[1][i] = a * letice[1][i];
	}*/
	letice[2][0] = letice[0][1] * letice[1][2] - letice[0][2] * letice[1][1];
	letice[2][1] = letice[0][2] * letice[1][0] - letice[0][0] * letice[1][2];
	letice[2][2] = letice[0][0] * letice[1][1] - letice[0][1] * letice[1][0];
	double pingfanghe = pow(letice[2][0], 2) + pow(letice[2][1], 2) + pow(letice[2][2], 2);
	double a = pow((pow(10 + distance_cha, 2) / pingfanghe), 0.5);	
	for (i = 0; i < 3; i++)
	{
		letice[2][i] = a * letice[2][i];
	}
	if (reverse_matrix(letice, 3, letice_r) == 0)
		return 0; //建立了逆矩阵

	//到这里完成了基矢的获取，下面是确定需要放哪个原子，以及新原子的位置是哪里
	//这里需要注意，我们首先需要知道偏移了多少，然后才能根据偏移方向针对性的写出原子真实坐标

	//现在遇到了问题，就是实际上放进去的原子个数应该是多余一个晶胞的，或者是不止一个晶胞个数，而应该要把包括的都放进去
	//这里我们这么做，先全部放进去。然后筛选
	for (i = 0; i < edge_num; i++)
	{
		if (list[i] == -1)
			break;
		for (m = 0; m < 3; m++)
		{
			fenshu_plus[i][m] = (cell_a.real_position[list[i] / cell_a.num][list[i] % cell_a.num][0] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][0]) * letice_r[0][m] + (cell_a.real_position[list[i] / cell_a.num][list[i] % cell_a.num][1] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][1]) * letice_r[1][m] + (cell_a.real_position[list[i] / cell_a.num][list[i] % cell_a.num][2] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][2]) * letice_r[2][m];
		}
		if (fenshu_plus[i][0] >= 0.9 || fenshu_plus[i][0] < -0.1)
		{
			fenshu_plus[i][0] = -100;
		}
		if (fenshu_plus[i][0] != -100)
			atom_plus++;
	}
	//cout << "now the atom plus is :" << atom_plus << endl;

	//for (i = 0; i < cell_a.num; i++)
	//{
	//	int m = 0;
	//	if (atom_flag_a[i] == 1) //如果确认了是这个原子需要放进来的话，我们求一下新的原子坐标
	//	{
	//		for (m = 0; m < 3; m++)
	//		{
	//			fenshu[j][m] = cell_a.real_position[k / cell_a.num][i][0] * letice_r[0][m] + cell_a.real_position[k / cell_a.num][i][1] * letice_r[1][m] + cell_a.real_position[k / cell_a.num][i][2] * letice_r[2][m];
	//		}
	//		j++;
	//	}
	//}
	//double sauare_rule = 10000;
	//double temp_sauare = 0;
	//for (i = 0, k = 0; i < copy_num; i++)
	//{
	//	for (j = 0; j < 3; j++)
	//	{
	//		temp_sauare += pow(cell_a.real_position[copy[i] / cell_a.num][copy[i] % cell_a.num][j], 2);
	//	}
	//	if (cell_a.p_real[copy[i] / cell_a.num][copy[i] % cell_a.num][0] > 0 && cell_a.p_real[copy[i] / cell_a.num][copy[i] % cell_a.num][1] > 0 && cell_a.p_real[copy[i] / cell_a.num][copy[i] % cell_a.num][2] > 0 && temp_sauare < sauare_rule)
	//	{
	//		sauare_rule = temp_sauare;
	//		k = copy[i];
	//	}
	//}
	////现在我找到了是那个最近邻的晶胞内部，下面开始反变换基矢坐标
	//for (i = 0; i < cell_a.num; i++)
	//{
	//	int m = 0;
	//	if (atom_flag_a[i] == 1) //如果确认了是这个原子需要放进来的话，我们求一下新的原子坐标
	//	{
	//		for (m = 0; m < 3; m++)
	//		{
	//			fenshu[j][m] = cell_a.real_position[k / cell_a.num][i][0] * letice_r[0][m] + cell_a.real_position[k / cell_a.num][i][1] * letice_r[1][m] + cell_a.real_position[k / cell_a.num][i][2] * letice_r[2][m];
	//		}
	//		j++;
	//	}
	//}
	//反变换之后开始进行c方向偏移
	double c_pingyi = 100;
	i = 0, j = 0;
	while (i < edge_num)
	{
		if (list[i] == -1)
			break;
		if (fenshu_plus[i][0] != -100)
		{
			if (fenshu_plus[i][2] < c_pingyi)
			{
				c_pingyi = fenshu_plus[i][2];
			}
		}
		i++;
	}

	for (i = 0; i < edge_num; i++)
	{
		if (list[i] == -1)
			break;
		if (fenshu_plus[i][0] != -100)
		{
			fenshu_plus[i][2] = fenshu_plus[i][2] - c_pingyi + 0.02;
		}
	}

	//cin.get();

	//然后我生成了新的坐标，开始写文件吧
	FILE *out = fopen(name.c_str(), "wt");
	char atom_head[20] = "atoms";
	fprintf(out, "%d\n", atom_plus);

	//fprintf(out, "%s\n", atom_head);
	fprintf(out, "%s\n", file[0].c_str());
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			fprintf(out, "\t%.9lf\t", letice[i][j]);
		}
		fprintf(out, "\n");
	}
	fprintf(out, "%s\n", file[1].c_str());
	j = 0;
	for (i = 0; i < edge_num; i++)
	{
		if (list[i] == -1)
			break;
		int mm = 1;
		if (fenshu_plus[i][0] != -100)
		{
			fprintf(out, "  %d\t", cell_a.type[list[i] % cell_a.num]);
			for (int m = 0; m < 3; m++)
			{
				fprintf(out, "%.9lf\t", fenshu_plus[i][m]);
			}
			for (int m = 0; m < 3; m++)
			{
				fprintf(out, "%d\t", mm);
			}
			fprintf(out, "\n");
		}
	}
	fclose(out);
	for (i = 0; i < atom_num_write; i++)
	{
		delete[] fenshu[i];
	}
	delete[] fenshu;
	for (i = 0; i < 3; i++)
	{
		delete[] letice_r[i];
	}
	delete[] letice_r;
	delete[] copy;
	for (i = 0; i < 3; i++)
	{
		delete[] letice[i];
	}
	delete[] letice;
	delete[] atom_flag_a;
	for (i = 0; i < edge_num; i++)
	{
		delete[] fenshu_plus[i];
	}
	delete[] fenshu_plus;
	return 1;
}

double get_line_dis(int *list, int edge_num, int orig, cell cell_a, int *copy)
{
	int i = 0, j = 0, k = 0;
	//这里的程序实现这样的功能，找到这个链上最长的距离
	for (i = 0; i < edge_num; i++)
	{
		if (list[i] == -1)
			break;
		else
		{
			if (list[i] % cell_a.num == orig)
			{
				j++;
			}
		}
	}
	double *distance = new double[j];
	double temp = 0;
	double max_dis = 0;
	for (i = 0; i < j; i++)
	{
		distance[i] = 200;
	}
	for (i = 0; i < j; i++)
	{
		for (k = 0; k < edge_num; k++)
		{
			if (list[k] == -1)
				break;
			temp = dis(cell_a.real_position[copy[i] / cell_a.num][copy[i] % cell_a.num], cell_a.real_position[list[k] / cell_a.num][list[k] % cell_a.num]);
			if (temp > 1e-3 && list[k] % cell_a.num != orig && temp < distance[i])
			{
				distance[i] = temp;
			}
		}
	}
	//到这里找出了每个最小的distance
	for (i = 0; i < j; i++)
	{
		if (distance[i] > max_dis)
			max_dis = distance[i];
	}

	delete[] distance;
	return max_dis;
}


int Comp(const void *p1, const void *p2)
{
	//自定义一个排序函数
	if ( ((struct three_double*)p1)->distnace < ((struct three_double*)p2)->distnace)
	{
		return 1;
	}	
	return 0;
}


int *judge_the_0d(cell cell_a, int *save_list, int edge_num, int *judge,int** expand_graph) //���������������������õ���ÿ����ͨ�������ж����ǲ�������Һõ�2d��ͨ����
{
	int i = 0, j = 0, k = 0, m = 0;
	int *pt = judge;
	int exit_flag = 0; //�˳���־��1��ʾ1d��2��ʾ2d��3��ʾ3d,0��ʾ���ڵ�̫���ڷ�ɢ����޷���������жϵ�����
	int *copy = new int[yanshen];
	for (int i = 0; i < yanshen; i++)
	{
		copy[i] = -1;
	}
	//�������ʵ�copy
	int temp = 0;
	for (i = 0; i < edge_num; i++)
	{
		if (save_list[i] != -1)
		{
			temp = save_list[i] % cell_a.num;
			for (k = 0; k < edge_num; k++)
			{
				if (save_list[k] == -1)
					break;
				if ((save_list[k]) % cell_a.num == temp)
				{
					copy[j] = save_list[k];
					j++;
				}
			}
			//cout << j << endl;
			break;
				
		}
		else
			break;
	}
	//0d的条件，复制体只有1个，不和其他接触，同时还要求不是散点
	int connect_num = 0;
	if (j == 1)
	{
		for (i = 0; i < edge_num; i++)
		{
			if (i != copy[0] && expand_graph[copy[0]][i] == 1)
			{
				connect_num++;
			}
		}
		if (connect_num > zeroD_atom_num_rule)
		{
			*pt = 0;
			*(pt + 1) = copy[0]%cell_a.num;
			delete[] copy;
			return pt;
		}
		else
		{
			*pt = -1;
			*(pt + 1) = -1;
			delete[] copy;
			return pt;
		}
			
	}
	else
	{
		*pt = -1;
		*(pt + 1) = -1;
		delete[] copy;
		return pt;
	}
}

int get_tablefor_1dchemistry(string&path, string& file_name,int num,element*e)
{
	
	int flag[120] = { 0 };
	qsort((void *)&e[1], 103, sizeof(class element), comp_nega);
	int yueshu = 1;
	int i = 0, j = 0;
	ifstream fin;
	fin.open(file_name, ios::in);
	if (!fin.is_open())
	{
		cout << "i can not find the file:name" << endl;
		cin.get();
	}
	string name;
	ofstream fout;
	fout.open("1d_analyse", ios::out);
	fout << "name" << "\t" << "all_name" << "\t" << "simplest_name" << endl;
	string result;
	for (i = 0; i < num; i++)
	{
		bool exit = false;
		fin >> name;
		fout << name << "\t";
		cell cell_a(const_cast<char*>((path + name).c_str()));
		fout << get_formula_name(cell_a) << "\t";
		//下面开始真正的输出名字
		
		for (j = 0; j < cell_a.num; j++)
		{
			flag[cell_a.type[j]]++;
		}
		while (exit==false)
		{
			for (j = 0; j < 120; j++)
			{
				/*if (flag[j] != 0)
				{
					cout << flag[j] << endl;
				}*/
				
				if (flag[j] != 0 && flag[j] % yueshu != 0)
				{
					exit = true;
				}
			}
			yueshu++;
			//cout << yueshu << endl;
		}
		yueshu = yueshu - 2;
		//cout << yueshu << endl;
		for (j = 0; j < 120; j++)
		{
			if (flag[j] != 0)
			{
				flag[j] = flag[j] / yueshu;
				//cout <<flag[j] << endl;
			}
		}
		//然后按照电负性顺序拼接起来
		/*for (i = 1; i < 103; i++)
		{
			cout << e[i].electron_negativity << "\t" << e[i].name << endl;
		}*/
		
		//先检查一下是不是排好序了
		/*for (i = 1; i < 103; i++)
		{
			cout << e[i].electron_negativity << "\t" << e[i].atomic_num<< endl;
		}
		cin.get();*/
		
		for (j = 1; j < 104; j++)
		{
			if (flag[e[j].atomic_num] != 0)
			{
				result += atom_name[e[j].atomic_num];
				result += to_string(flag[e[j].atomic_num]);
			}
		}
		//cout << result << endl;
		fout << result << endl;
		result.clear();
		
		for (j = 0; j < 120; j++)
		{
			flag[j] = 0;
		}
		yueshu = 1;
		cout << "has generate :" << name << endl;
	}



	fout.close();
	fin.close();
	return 1;
}
string  get_formula_name(cell& cell_a)
{
	int i = 0;
	int flag[120] = { 0 };
	for (i = 0; i < cell_a.num; i++)
	{
		flag[cell_a.type[i]]++;
	}
	string name;
	for (i = 0; i < 120; i++)
	{
		if (flag[i] != 0)
		{
			name += atom_name[i];
			name += to_string(flag[i]);
		}

	}
	//cout << name << endl;
	//cin.get();
	return name;
}

int comp_nega(const void *p1, const void *p2)
{
	//注意这个是从大到小
	if (((element *)p1)->electron_negativity < ((element *)p2)->electron_negativity)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int two_stu_check(cell &cell_a, int* list, int num)
{
	//输入是点的序号，输出是这个2d结构合不合适，需要穿过中间格子
	int flag = 0;
	for (int i = 0; i < num; i++)
	{
		if (list[i] == -1)
			break;
		else
		{
			if (list[i] / cell_a.num == (yanshen - 1) / 2)
			{
				flag = 1;
				break;
			}
		}
	}
	return flag;
}