#include "CONNECTION.h"
#include <fstream>
#include <iostream>
#include <string>
#include<vector>
#pragma warning(disable : 4996)
using namespace std;

static char wenjian[100] = "431400";
static char result_path[40] = "431400";
static char style[100] = "rule_noorg_for_pc.txt";
static char wenjian_real[100];

const char a[120][3] = { " ", "H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th", "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt", "Ds", "Rg" };
const int AIM = 2;//标志寻找几维材料，1是1d，2是2d，0是0d

int main(int argc, const char *argv[])
{
	//先读取各种信息
	int i = 0, j = 0, k = 0, l = 0, m = 0;
	//string supporting_data_path="H:\\my_code\\ionc_ridus_series\\ionc_ridus_series\\data\\";
	//string supporting_data_path ;
	string supporting_data_path = "/share/home/wangz/projects/new_connection1231/bin/x64/Debug/";
	string file_name1 = supporting_data_path + "input_ionic";
	string file_name2 = supporting_data_path + "input_ionic_plus";
	string file_element_r = supporting_data_path + "ridus";
	string file_colvance = supporting_data_path + "colvance";
	string file_file_nagetivity = supporting_data_path + "negativity.txt";
	string file_first_ionization_energy = supporting_data_path + "first_ionazation_energy.txt";
	string max_ionic = supporting_data_path + "max_ionic";
	string data_path = "/share/home/wangz/icsd_2019/icsd_2019/";
	string zerod_result_path = "/share/home/wangz/2d_search/ridus_cut/result/result_ionicandval/0103/my_0d/";
	////建立离子半径的存储结构	
	/*ionic_radius***** ir;
	ir = new ionic_radius****[120];
	for (i = 0; i < 120; i++)
	{
		ir[i] = new ionic_radius***[20];
		for (j = 0; j < 20; j++)
		{
			ir[i][j] = new ionic_radius**[15];
			for (k = 0; k < 15; k++)
			{
				ir[i][j][k] = new ionic_radius*[3];
				for (l = 0; l < 3; l++)
				{
					ir[i][j][k][l] = new ionic_radius[3];
				}

			}
		}
	}*/
	double **dist = new double*[120];
	for (i = 0; i < 120; i++)
	{
		dist[i] = new double[120];
		for (j = 0; j < 120; j++)
		{
			dist[i][j] = -100;
		}
	}
	//建立元素的	
	element *e;
	e = new element[120];
	//开始依次读取相关信息
	//read_radius(ir, file_name1, file_name2);
	read_element(e, file_element_r, file_colvance, file_file_nagetivity, file_first_ionization_energy);


	////判断这8千个结构是不是存在共价键
	//ofstream fout;
	//fout.open("new_null_bulk", ios::out);
	//ifstream fin;
	//fin.open("null_bulk", ios::in);
	//if (!fin.is_open())
	//{
	//	cout << "i can not find the file!  null_bulk" << endl;
	//	cin.get();
	//}
	//string name_temp;
	//i = 0;
	//while (fin.good() && fin.peek() != EOF)
	//{
	//	fin >> name_temp;
	//	cout << "has gone" << i ++<< "file!" << endl;
	//	cell cell_a(const_cast<char*>((data_path + name_temp+"_1/atom.config").c_str()));
	//	if(if_valence_conncet(cell_a, e) == true)
	//	{
	//		fout << name_temp << endl;
	//	}
	//}
	//fin.close();
	//fout.close();
	//cout << "new null file done!" << endl;
	//cin.get();
	//return 1;

	//根据元素找出对应元素正价和负价的最大值半径储存下来
	ifstream fin;
	fin.open(max_ionic, ios::in);
	vector<vector<double>> max_ionic_riuds(120);
	for (i = 0; i < 120; i++)
	{
		max_ionic_riuds[i].resize(2);
		max_ionic_riuds[i][0] = -1;
		max_ionic_riuds[i][1] = -1;
		fin >> max_ionic_riuds[i][0];
		fin >> max_ionic_riuds[i][1];

	}
	fin.close();
	//for (i = 0; i < 120; i++)
	//{
	//	double max_1 = -999;
	//	double max_2 = -999;

	//	//首先是负价半径找寻最大值
	//	for (j = 0; j < 8; j++)
	//	{
	//		for (k = 0; k < 15; k++)
	//		{
	//			for (l = 0; l < 3; l++)
	//			{
	//				for (m = 0; m < 3; m++)
	//				{
	//					if (ir[i][j][k][l][m].radius > max_2)
	//					{
	//						max_2 = ir[i][j][k][l][m].radius;
	//					}
	//				}
	//			}
	//		}
	//	}
	//	max_ionic_riuds[i][1] = max_2;
	//	//然后是正价半径寻找最大值
	//	for (j = 8; j < 20; j++)
	//	{
	//		for (k = 0; k < 15; k++)
	//		{
	//			for (l = 0; l < 3; l++)
	//			{
	//				for (m = 0; m < 3; m++)
	//				{
	//					if (ir[i][j][k][l][m].radius > max_1)
	//					{
	//						max_1 = ir[i][j][k][l][m].radius;
	//					}
	//				}
	//			}
	//		}
	//	}
	//	max_ionic_riuds[i][0] = max_1;
	//	cout << "element: " << a[i] << ":" << max_1 << "," << max_2 << endl;
	//	//cin.get();
	//}

	ofstream fout;
	//fout.open("max_ionic", ios::out);
	//for (i = 0; i < 120; i++)
	//{
	//	fout << max_ionic_riuds[i][0] << "\t" << max_ionic_riuds[i][1] << endl;
	//}
	//fout.close();

	////然后输出一个用于展示的半径表，
	/*fout.open("max_show", ios::out);
	fout << "element  " << "number  " << "coval_ridus  " << "positive_ridus  " << "negative_r  " <<"metal_ridus" <<endl;
	for (i = 1; i < 104; i++)
	{
		cout << i << endl;
		fout << a[i] << "  " << i << "  " << e[i].cov_radius << "  " << max_ionic_riuds[i][0] << "  " << max_ionic_riuds[i][1] << "  " << e[i].metal_radius[0] <<  endl;
	}
	fout.close();*/
	//输出主族元素之间的离子半径和共价半径加和的比较
	/*fout.open("check", ios::out);
	for (i = 0; i < main_groupnum; i++)
	{
		for (j = i+1; j < main_groupnum; j++)
		{
			if (i == j)
				continue;
			if (e[main_group_element[i]].electron_negativity > e[main_group_element[j]].electron_negativity)
			{
				double puls = max_ionic_riuds[main_group_element[i]][1] + max_ionic_riuds[main_group_element[j]][0];
				double cov = (e[main_group_element[i]].cov_radius + e[main_group_element[j]].cov_radius) / 100.0;
				fout << atom_name[main_group_element[i]] << "  " << atom_name[main_group_element[j]] << " ";
				fout << puls << "  " << cov << endl;
			}
			else
			{
				double puls = max_ionic_riuds[main_group_element[i]][0] + max_ionic_riuds[main_group_element[j]][1];
				double cov = (e[main_group_element[i]].cov_radius + e[main_group_element[j]].cov_radius) / 100.0;
				fout << atom_name[main_group_element[i]] << "  " << atom_name[main_group_element[j]] << " ";
				fout << puls << "  " << cov << endl;
			}
		}
	}
	fout.close();
	cout << "done!" << endl;
	cin.get();
	return 0;*/




	//一个小功能，输入是一个路径和文件名字，输出是化学表达式
	/*string oned_path = "/share/home/wangz/2d_search/ridus_cut/result/result_ionicandval/0103/my_1d/";
	string oned_name = "/share/home/wangz/2d_search/ridus_cut/result/result_ionicandval/0103/my_1d/name";
	get_tablefor_1dchemistry(oned_path, oned_name, 1569, e);
	cout << "analyse 1d stu complete!" << endl;
	cin.get();*/




	double temp = 0.0;
	for (i = 0; i < argc; i++)
	{

		if (i == 1)
		{
			for (j = 0; j < strlen(argv[i]); j++)
			{
				wenjian[j] = argv[i][j];
				//cout<<argv[i][j]<<endl;
				//cout<<wenjian[j]<<endl;
			}
			wenjian[strlen(argv[i])] = '\0';
		}
		if (i == 2)
		{
			for (j = 0; j < strlen(argv[i]); j++)
			{
				result_path[j] = argv[i][j];
			}
			result_path[strlen(argv[i])] = '\0';
		}

		/*if (i == 4)
		{
			for (j = 0; j < strlen(argv[i]); j++)
			{
				wenjian_real[j] = argv[i][j];
			}
			wenjian_real[strlen(argv[i])] = '\0';
		}*/
	}

	char path_flag[200];
	strcpy(path_flag, wenjian);
	//strcat(wenjian, ".cif.config");
	cell cell_a(wenjian, e, max_ionic_riuds, 2);
	/*for (i = 0; i < cell_a.num; i++)
	{
		cout << i<<","<<cell_a.ridus[i] << endl;
	}
	cin.get();*/
	//if (cell_a.num == 0 || judge_falg_file(path_flag) == 0)
	//	return 0;

	//建立有关成键可能性的记录
	//第一个数是
	double ***possiblty = new double **[cell_a.type_num];
	for (i = 0; i < cell_a.type_num; i++)
	{
		possiblty[i] = new double *[cell_a.type_num];
		for (j = 0; j < cell_a.type_num; j++)
		{
			possiblty[i][j] = new double[3];
			possiblty[i][j][0] = -999;
			possiblty[i][j][1] = 100;
		}
	}

	int num = cell_a.num * yanshen;
	save save_a(num);
	save *pt = &save_a; //ָ��ָ��save_a	
	int *ex_visited; //�����Ƿ�������������
	ex_visited = new int[num];
	int **expand_graph = new int *[num]; //��ά���󣬴��������֮�����ϵ
	for (i = 0; i < num; i++)
	{
		expand_graph[i] = new int[num];
		ex_visited[i] = 0;
	}
	for (i = 0; i < num; i++)
	{
		for (j = 0; j < num; j++)
		{
			expand_graph[i][j] = -1;
		}
	}
	//��ʼ������ͨ�Ե��ж�
	full_expand_generate_graph(cell_a, expand_graph, num, e, max_ionic_riuds, dist);
	//补充完最后一项,最后记录
	int poss_save[2];
	double min_temp = 200;
	for (i = 0; i < cell_a.type_num; i++)
	{
		for (j = 0; j < cell_a.type_num; j++)
		{
			if (i != j)
			{
				possiblty[i][j][2] = abs(possiblty[i][j][0] - possiblty[i][j][1]);
				if (possiblty[i][j][2] < min_temp)
				{
					min_temp = possiblty[i][j][2];
					poss_save[0] = i;
					poss_save[1] = j;
				}
			}
		}
	}

	ex_find_connect(num, cell_a, ex_visited, expand_graph, pt);
	//cout << "the subnets is :" << pt->sunbets << endl;
	int success_ful = 0;
	if (save_a.sunbets == 1)
	{
		//cout << "this atom do not have the graph_connection!";
		//generate_testfile(pt, cell_a, num);
		delete[] ex_visited;
		for (i = 0; i < num; i++)
		{
			delete[] expand_graph[i];
		}
		delete[] expand_graph;
		//cout << "all work done" << endl;
		//cin.get();
		return success_ful;
	}
	//�õ�����ͨ����Ϊ1˵���϶��ǲ������������ˣ�ֱ��������
	//�������ÿ�ֵõ�����ͨ���������ж�

	else
	{
		//cout << "this atom has :" << pt->sunbets << " graph_connection!,please check!" << endl;
		int **judge = new int *[pt->sunbets];
		for (int i = 0; i < pt->sunbets; i++)
		{
			judge[i] = new int[2];
		}


		if (AIM == 2)
		{
			//现在建立一个标志位用来标志二维的连通分量是不是两头的，0表示起始，1表示中间，2表示末尾，-1表示不是2d
		//我们现在做这样的事情，建立二维连通分量只针对中间的来建立

			//测试下如果通过，穿过中间格子的方法来改进行不行
			int *middle_flag = new int[pt->sunbets];
			for (i = 0; i < pt->sunbets; i++)
			{
				middle_flag[i] = 0;
			}
			//���������ͨ�����������жϽ��

			for (j = 0; j < pt->sunbets; j++)
			{

				//cout << "now it's judge the :" << j << endl;
				judge[j][0] = 0;
				judge[j] = judge_the_2d(cell_a, pt->save_list[j], num, judge[j]);
				if (judge[j][0] == 2)
					success_ful = 1;
			}
			//	//cout << "the judge result is :" << judge[j][0] << judge[j][1] << endl;
			//}
		    return success_ful;


			for (j = 0; j < pt->sunbets; j++)
			{

				//cout << "now it's judge the :" << j << endl;
				judge[j][0] = 0;
				judge[j] = judge_the_2d(cell_a, pt->save_list[j], num, judge[j]);
				if (judge[j][0] == 2&& two_stu_check(cell_a, pt->save_list[j], num)==1)
				{
					middle_flag[j] = two_stu_check(cell_a, pt->save_list[j], num);
					success_ful = 1;
				}				
				//cout << "the judge result is :" << judge[j][0] << judge[j][1] << endl;
			}
			if (success_ful == 0)
				return 0;

			//这里我们重写一下关于两边的判断
			int twod_num = 0; //先看一下有多少个2d的连通分量
			j = 0;

			for (i = 0; i < pt->sunbets; i++)
			{
				if (judge[i][0] == 2)
				{
					twod_num++;
				}
			}
			//if (twod_num == pt->sunbets) //只有在全2d的情况下才决定两边不要
			//{
			//	//先建立好该需要的空间
			//	double *angle_face = new double[pt->sunbets]; //储存每个连通分量与其他分量角度的最小值
			//	int *xuhao_face = new int[pt->sunbets];		  //储存对应序号
			//	double **temp_angel_save = new double *[2];   //临时储存用来计算角度的中间变量
			//	for (i = 0; i < 2; i++)
			//	{
			//		temp_angel_save[i] = new double[3];
			//	}
			//	int *xuhao_a = new int[pt->sunbets]; //临时储存序号
			//	for (i = 0; i < pt->sunbets; i++)
			//	{
			//		xuhao_a[i] = i;
			//	}
			//	int **random_choose = new int *[pt->sunbets * (pt->sunbets - 1) / 2]; //储存任选两个的序号
			//	for (i = 0; i < pt->sunbets * (pt->sunbets - 1) / 2; i++)
			//	{
			//		random_choose[i] = new int[2];
			//	}
			//	random_choose_two(xuhao_a, pt->sunbets, random_choose);

			//	for (i = 0; i < pt->sunbets; i++) //每次需要找到这堆角度的最大值
			//	{
			//		//每次我需要记录三个序号
			//		int now_xuhao = pt->save_list[i][0];
			//		;
			//		int other1_xuhao;
			//		int oth2_xuhao;
			//		double temp_min_angel = -2;
			//		double every_angel;
			//		for (j = 0; j < pt->sunbets * (pt->sunbets - 1) / 2; j++)
			//		{
			//			if (random_choose[j][0] != i && random_choose[j][1] != i) //取出两个另外的来
			//			{
			//				other1_xuhao = pt->save_list[random_choose[j][0]][0];
			//				oth2_xuhao = pt->save_list[random_choose[j][1]][0];
			//				//cout << random_choose[j][0] << "," << random_choose[j][1] << endl;
			//			}
			//			else
			//				continue;
			//			//选出来之后计算角度
			//			for (k = 0; k < 3; k++)
			//			{
			//				temp_angel_save[0][k] = cell_a.real_position[other1_xuhao / cell_a.num][other1_xuhao % cell_a.num][k] - cell_a.real_position[now_xuhao / cell_a.num][now_xuhao % cell_a.num][k];
			//				temp_angel_save[1][k] = cell_a.real_position[oth2_xuhao / cell_a.num][oth2_xuhao % cell_a.num][k] - cell_a.real_position[now_xuhao / cell_a.num][now_xuhao % cell_a.num][k];
			//			}
			//			every_angel = vector_angle(temp_angel_save);
			//			//cout << every_angel << endl;
			//			if (every_angel > temp_min_angel) //需要的是最大值
			//			{
			//				temp_min_angel = every_angel;
			//			}
			//		}
			//		angle_face[i] = temp_min_angel;
			//		temp_min_angel = -2;
			//		//cout << angle_face[i]<< endl;
			//	}

			//	//在最大值里面找到最小值
			//	buble_plus(angle_face, xuhao_face, pt->sunbets);
			//	/*cout << angle_face[0] << angle_face[1] << endl;
			//	cout << xuhao_face[0] << xuhao_face[1] << endl;
			//	middle_flag[xuhao_face[0]] = 0;
			//	middle_flag[xuhao_face[1]] = 2;*/
			//	for (i = 2; i < pt->sunbets; i++)
			//	{
			//		middle_flag[xuhao_face[i]] = 1;
			//	}

			//	for (i = 0; i < pt->sunbets * (pt->sunbets - 1) / 2; i++)
			//	{
			//		delete[] random_choose[i];
			//	}
			//	delete[] random_choose;
			//	delete[] xuhao_a;
			//	for (i = 0; i < 2; i++)
			//	{
			//		delete[] temp_angel_save[i];
			//	}
			//	delete[] temp_angel_save;
			//	delete[] angle_face;
			//	delete[] xuhao_face;
			//}
			//else
			//{
			//	for (i = 0; i < pt->sunbets; i++)
			//	{
			//		middle_flag[i] = 1;
			//	}
			//}

			//test middle falg
			/*for (i = 0; i < pt->sunbets; i++)
			{
				cout << middle_flag[i] << endl;
			}*/

			////然后需要输出层之间的最短距离。
			double temp_layer_dis = 0;
			double *layer_dis = new double[pt->sunbets];
			string **show_ele = new string *[pt->sunbets];
			double *duiying_style = new double[pt->sunbets];

			for (i = 0; i < pt->sunbets; i++)
			{
				show_ele[i] = new string[2];
			}

			for (int i = 0; i < pt->sunbets; i++)
			{
				layer_dis[i] = 100.0;
			}


			//这里输出每个2d结构距离周围其他原子rule和最长距离
			for (j = 0; j < pt->sunbets; j++)
			{
				if (judge[j][0] == 2 && middle_flag[j] == 1)
				{
					for (i = 0; i < num && pt->save_list[j][i] != -1; i++)
					{
						//找到了每个2d连通分量的序号save_list[j][i]
						//对应找到的是其他的联通分量
						for (int n = 0; n < pt->sunbets; n++)
						{
							if (n != j)
							{
								for (int m = 0; m < num && pt->save_list[n][m] != -1; m++)
								{
									if (expand_graph[pt->save_list[n][m]][pt->save_list[j][i]] != 1)
									{
										temp_layer_dis = dis(cell_a.real_position[(yanshen-1)/2][pt->save_list[j][i] % cell_a.num], cell_a.real_position[(yanshen - 1) / 2][pt->save_list[n][m] % cell_a.num]);
										if (temp_layer_dis < layer_dis[j] && temp_layer_dis > 1e-3)
										{
											//cout << a[cell_a.type[pt->save_list[j][i] % cell_a.num]] << "," << a[cell_a.type[pt->save_list[n][m] % cell_a.num]] << ",";
											//cout << pt->save_list[j][i] % cell_a.num << "," << pt->save_list[n][m] % cell_a.num << endl;
											layer_dis[j] = temp_layer_dis;
											show_ele[j][0] = a[cell_a.type[pt->save_list[j][i] % cell_a.num]];
											show_ele[j][1] = a[cell_a.type[pt->save_list[n][m] % cell_a.num]];
											duiying_style[j] = dist[cell_a.type[pt->save_list[j][i] % cell_a.num]][cell_a.type[pt->save_list[n][m] % cell_a.num]];
											
											//cout << "now the lauer:" << j << "is:" << temp_layer_dis;
										}
									}
								}
							}
						}
						
												
					}
					
				}

				//只需要找到一层就行
			}
			////到这里我们找到了最近距离
			ofstream fout;
			char dis_name[100] = ".txt";
			char dis_name_real[300];
			strcpy(dis_name_real, result_path);
			fout.open(strcat(dis_name_real, dis_name), ios::out);
			/*if (!fout.is_open())
			{
				cout << "filed to open the file!" << endl;
				cout << "now the file path is :" << dis_name_real << endl;
				cin.get();
			}*/

			//然后开始根据我们的判断结果进行生成新的atom.config文件了
			//如果根据连通性获得了多个，我们不是记录了最多的，同时分别生成对应的2d文件

			int *generate_flag = new int[pt->sunbets]; //标记是不是需要写进文件0的话不写
			for (i = 0; i < pt->sunbets; i++)
			{
				generate_flag[i] = 1;
			}
			success_ful = 0;
			//fout << wenjian_real << "\n";
			char two_filename[100];
			int towd_num = 0;
			char temp_num[20];
			for (j = 0; j < pt->sunbets; j++)
			{
				if (judge[j][0] == 2 && generate_flag[j] == 1 && middle_flag[j] == 1)
				{
					for (int i = 0; i < pt->sunbets; i++)
					{
						if (judge[j][1] == judge[i][1])
							generate_flag[i] = 0;
					}
					//two_filename = wenjian + '-' + int2string(towd_num) + "_2d.config";
					sprintf(temp_num, "%d", towd_num);
					strcpy(two_filename, result_path);
					//strcat(two_filename, wenjian);
					strcat(two_filename, "-");
					strcat(two_filename, temp_num);
					strcat(two_filename, "_2d.config");

					if (generate_atom(pt, judge[j], num, cell_a, two_filename, pt->save_list[j]) == 1)
					{
						//cout << two_filename << endl;
						success_ful = 1;
						fout << show_ele[j][0] << "\t" << show_ele[j][1] << "\t" << layer_dis[j] << "\t" << duiying_style[j];
						fout << "\t" << double(layer_dis[j] / duiying_style[j]) << endl;
						/*fout << rule_max << "\t" << no_connect << "\t" << abs(rule_max - no_connect) << endl;
						fout << possiblty[poss_save[0]][poss_save[1]][0] << "\t" << possiblty[poss_save[0]][poss_save[1]][1] << "\t" << possiblty[poss_save[0]][poss_save[1]][2] << endl;
						*/
						//break;
						towd_num++;

					}
					
				}
			}

			//generate_outfile(cell_a, pt, num, judge);
			//fout << endl;
			fout.close();
			delete[] generate_flag;
			//delete[] layer_dis;
			//delete[] duiying_style;
			delete[] middle_flag;
			//cin.get();
		}
		else if (AIM == 1)
		{
			//cout << "show the connections xuhao!" << endl;
			/*for (i = 0; i < pt->sunbets; i++)
			{
				int num_point = 0;
				for (j = 0; j < num; j++)
				{
					if (pt->save_list[i][j] != -1)
					{
						num_point++;
						cout << pt->save_list[i][j]<<",";
					}
				}
				cout << num_point << endl;
				cout << endl;
			}
			cin.get();*/
			for (j = 0; j < pt->sunbets; j++)
			{

				//cout << "now it's judge the :" << j << endl;
				judge[j][0] = 0;
				judge[j] = judge_the_1d(cell_a, pt->save_list[j], num, judge[j]);
				if (judge[j][0] == 1)
					success_ful = 1;
				//cout << "the judge result is :" << judge[j][0] << judge[j][1] << endl;
			}
			//cin.get();
			if (success_ful == 0)
				return 0;

			//然后需要输出层之间的最短距离。
			double temp_layer_dis = 0;
			double *layer_dis = new double[pt->sunbets];
			string **show_ele = new string *[pt->sunbets];
			double *duiying_style = new double[pt->sunbets];

			for (i = 0; i < pt->sunbets; i++)
			{
				show_ele[i] = new string[2];
			}

			for (int i = 0; i < pt->sunbets; i++)
			{
				layer_dis[i] = 100.0;
			}

			for (j = 0; j < pt->sunbets; j++)
			{
				if (judge[j][0] == 1)
				{
					for (i = 0; i < num && pt->save_list[j][i] != -1; i++)
					{
						//找到了每个2d连通分量的序号save_list[j][i]
						for (int m = 0; m < pt->sunbets; m++)
						{
							if (m != j && judge[m][0] == 1)
							{
								for (int k = 0; k < num && pt->save_list[m][k] != -1; k++)
								{
									temp_layer_dis = dis(cell_a.real_position[pt->save_list[j][i] / cell_a.num][pt->save_list[j][i] % cell_a.num], cell_a.real_position[pt->save_list[m][k] / cell_a.num][pt->save_list[m][k] % cell_a.num]);
									if (temp_layer_dis < layer_dis[j] && temp_layer_dis > 1e-3)
									{
										layer_dis[j] = temp_layer_dis;
										show_ele[j][0] = a[cell_a.type[pt->save_list[j][i] % cell_a.num]];
										show_ele[j][1] = a[cell_a.type[pt->save_list[m][k] % cell_a.num]];
										duiying_style[j] = dist[cell_a.type[pt->save_list[j][i] % cell_a.num]][cell_a.type[pt->save_list[m][k] % cell_a.num]];
										//cout << "now the lauer:" << j << "is:" << temp_layer_dis;
									}
								}
							}
						}
					}
				}
			}
			//到这里我们找到了最近距离
			ofstream fout;
			char dis_name[100] = ".txt";
			char dis_name_real[200];
			strcpy(dis_name_real, result_path);
			fout.open(strcat(dis_name_real, dis_name), ios::out | ios::app);
			if (!fout.is_open())
			{
				cout << "filed to open the file!" << endl;
				cout << "now the file path is :" << dis_name << endl;
				cin.get();
			}

			//然后开始根据我们的判断结果进行生成新的atom.config文件了
			//如果根据连通性获得了多个，我们不是记录了最多的，同时分别生成对应的2d文件
			int *generate_flag = new int[pt->sunbets]; //标记是不是需要写进文件0的话不写
			for (i = 0; i < pt->sunbets; i++)
			{
				generate_flag[i] = 1;
			}
			success_ful = 0;
			//fout << wenjian_real << "\n";
			char two_filename[100];
			int towd_num = 0;
			char temp_num[20];
			for (j = 0; j < pt->sunbets; j++)
			{
				//这里我们需要注意，产生的1d结构就只能在中间部分，别跑到外面产生
				int cubic_num = pt->save_list[j][0] / cell_a.num;
				int layer_count = cengshu * cengshu;
				if (cubic_num / layer_count == 0 || cubic_num / layer_count == (cengshu - 1) || cubic_num % layer_count == 0 || cubic_num % layer_count == (cengshu - 1))
				{
					continue;
				}
				if (judge[j][0] == 1 && generate_flag[j] == 1)
				{
					for (int i = 0; i < pt->sunbets; i++)
					{
						if (judge[j][1] == judge[i][1])
							generate_flag[i] = 0;
					}
					//two_filename = wenjian + '-' + int2string(towd_num) + "_2d.config";
					sprintf(temp_num, "%d", towd_num);
					strcpy(two_filename, result_path);
					//strcat(two_filename, wenjian);
					strcat(two_filename, "-");
					strcat(two_filename, temp_num);
					strcat(two_filename, "_1d.config");
					
					if (generate_atom_1d(pt, judge[j], num, cell_a, two_filename, pt->save_list[j], expand_graph) != 0)
					{

						//cout << "look i got the 1d situation!" << endl;
						//cout << "the file is: " << wenjian << endl;
						//cin.get();
						success_ful = 1;

						fout << show_ele[j][0] << "\t" << show_ele[j][1] << "\t" << layer_dis[j] << "\t" << duiying_style[j];
						fout << "\t" << double(layer_dis[j] / duiying_style[j]) << endl;
						//break;
						towd_num++;
					}
					
				}
			}

			//generate_outfile(cell_a, pt, num, judge);
			//fout << endl;
			fout.close();

			for (int i = 0; i < pt->sunbets; i++)
			{				
				delete[] show_ele[i];
			}
			delete[] show_ele;			
			//delete[]subnets_num;
			delete[] generate_flag;
			delete[] layer_dis;
			delete[] duiying_style;
		}
		else if (AIM == 0)
		{
			//这里注意出现寻找1d的情况的时候，不产生结构文件，只是将结构复制到指定目录下
			string com;
			for (j = 0; j < pt->sunbets; j++)
			{

				//cout << "now it's judge the :" << j << endl;
				judge[j][0] = 0;
				judge[j] = judge_the_0d(cell_a, pt->save_list[j], num, judge[j],expand_graph);
				if (judge[j][0] == 0)
					success_ful = 1;
				//cout << "the judge result is :" << judge[j][0] << judge[j][1] << endl;
			}
			//cin.get();
			if (success_ful == 0)
				return 0;
			com = "cp " + string(wenjian)+ " " + zerod_result_path;
			system(com.c_str());

		}
		else
		{
			cout << "wrong AIM number!please check!" << endl;
			cin.get();
		}
		//下面这些是真的需要删掉的
		for (int i = 0; i < pt->sunbets; i++)
		{
			delete[] judge[i];
			//delete[] show_ele[i];
		}
		//delete[] show_ele;
		delete[] judge;
		delete[] ex_visited;
		for (i = 0; i < num; i++)
		{
			delete[] expand_graph[i];
		}
		delete[] expand_graph;
		//delete[]subnets_num;

		return success_ful;
	}
	//cout << "all total work done" << endl;
	//cin.get();
}