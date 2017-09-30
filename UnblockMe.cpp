#define _CRT_SECURE_NO_WARNINGS
# include "iGraphics.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <map>
#include <string>
#include <windows.h>
#define Relax if(!t[tem]){ Q.push(tem); prev[tem] = u; t[tem] = true; }

using namespace std;
vector <string> sol;
int xx=0, yy=0, mb, lvl, idx=0, st=0, moves=0, movemade=0, solvetokens, px, py, sound=1;
FILE *File = fopen("./Data/GameData.txt", "r");
char *img[5] = { "./Image/0.bmp", "./Image/1.bmp", "./Image/2.bmp", "./Image/3.bmp", "./Image/4.bmp" };
char *bs[10] = { "./Image/s0.bmp", "./Image/s1.bmp", "./Image/s2.bmp", "./Image/s3.bmp" ,"./Image/s4.bmp" ,"./Image/s5.bmp" ,"./Image/s6.bmp", "./Image/s7.bmp", "./Image/s8.bmp" ,"Image/s9.bmp"};
char *ds[3] = { "./Image/d0.bmp", "./Image/d1.bmp", "./Image/d2.bmp" };
char *ns[12] = { "./Image/n1.bmp", "./Image/n2.bmp", "./Image/n3.bmp", "./Image/n4.bmp", "./Image/n5.bmp", "./Image/n6.bmp", "./Image/n7.bmp", "./Image/n8.bmp", "./Image/n9.bmp", "./Image/n10.bmp", "./Image/n11.bmp", "./Image/n12.bmp" };
bool vis[10][8];
int solved = 0;
struct block{
	int x, y, t;
	void Draw(){
		iShowBMP(x * 64 - 44+ 376, y * 64, img[t], 0);
	}
	void moveup(){
		if (t == 1 && !vis[x][y + 2]){
			vis[x][y] = false;
			vis[x][y + 2] = true;
			y++;
		}
		if (t == 3 && !vis[x][y + 3]){
			vis[x][y] = false;
			vis[x][y + 3] = true;
			y++;
		}
	}
	void movedown(){
		if (t == 1 && !vis[x][y - 1]){
			vis[x][y + 1] = false;
			vis[x][y - 1] = true;
			y--;
		}
		if (t == 3 && !vis[x][y - 1]){
			vis[x][y + 2] = false;
			vis[x][y - 1] = true;
			y--;
		}
	}
	void moveleft(){
		if (t % 4 == 0 && !vis[x - 1][y]){
			vis[x + 1][y] = false;
			vis[x - 1][y] = true;
			x--;
		}
		if (t == 2 && !vis[x - 1][y]){
			vis[x + 2][y] = false;
			vis[x - 1][y] = true;
			x--;
		}
	}
	void moveright(){
		if (t % 4 == 0 && !vis[x + 2][y]){
			vis[x][y] = false;
			vis[x + 2][y] = true;
			x++;
		}
		if (t == 2 && !vis[x + 3][y]){
			vis[x][y] = false;
			vis[x + 3][y] = true;
			x++;
		}
	}
};

vector <block> v[36],vcpy[36];
bool locked[36];
int rmove[36];
void DrawBoard(){
	iClear();
	iShowBMP(0, 0, bs[4], 0);
	iShowBMP(485, 480, ds[lvl / 12], 0);
	iShowBMP(485+50, 485, ns[lvl % 12], 0);
	if (rmove[lvl] == -1){
		iText(240, 255, "--", GLUT_BITMAP_HELVETICA_18);
	}
	else{
		char str[5];
		sprintf_s(str, "%d", rmove[lvl]);
		iText(240, 255, str, GLUT_BITMAP_HELVETICA_18);
	}
	char smove[5];
	sprintf_s(smove, "%d", moves);
	iText(162, 227, smove, GLUT_BITMAP_HELVETICA_18);

	char token[5];
	sprintf_s(token, "%d", solvetokens);
	iText(260, 441, token, GLUT_BITMAP_HELVETICA_18);
	for (int i = 0; i < v[lvl].size(); i++){
		v[lvl][i].Draw();
	}
}

string GameString(vector <block> G){
	string s = "";
	for (int i = 0; i < G.size(); i++){
		s.push_back(G[i].x + '0');
		s.push_back(G[i].y + '0');
		s.push_back(G[i].t + '0');
	}
	return s;
}

void movetosolve(int i){
	if (sound) PlaySound("./Sound/tak.wav", NULL, SND_ASYNC);
	for (int j = 0; j < sol[i].size(); j += 3){
		if (sol[i][j] < sol[i + 1][j]){
			for (int l = 0; l < sol[i + 1][j] - sol[i][j]; l++) v[lvl][j / 3].moveright();
			DrawBoard();
			Sleep(500);
		}
		if (sol[i][j] > sol[i + 1][j]){
			for (int l = 0; l < sol[i][j] - sol[i + 1][j]; l++) v[lvl][j / 3].moveleft();
			DrawBoard();
			Sleep(500);
		}
		if (sol[i][j + 1] < sol[i + 1][j + 1]){
			for (int l = 0; l < sol[i + 1][j + 1] - sol[i][j + 1]; l++) v[lvl][j / 3].moveup();
			DrawBoard();
			Sleep(500);
		}
		if (sol[i][j + 1] > sol[i + 1][j + 1]){
			for (int l = 0; l < sol[i][j + 1] - sol[i + 1][j + 1]; l++) v[lvl][j / 3].movedown();
			DrawBoard();
			Sleep(500);
		}
	}
	moves++;
}

void solve(){
	queue <string> Q;
	map <string, bool> t;
	map <string, string> prev;
	Q.push(GameString(v[lvl]));
	t[Q.front()] = true;
	prev[Q.front()] = "";
	while (!Q.empty()){
		string u=Q.front();
		Q.pop();
		bool visit[8][8];
		memset(visit, false, sizeof(visit));
		for (int i = 0; i < 8; i++){
			visit[i][0] = true; visit[0][i] = true;
			visit[7][i] = true; visit[i][7] = true;
		}
		for (int i = 0; i < u.size(); i+=3){
			if ((u[i+2]-'0') % 4 == 0) visit[(u[i]-'0')][(u[i+1]-'0')] = true, visit[(u[i]-'0') + 1][(u[i+1]-'0')] = true;
			if ((u[i+2]-'0') == 1)  visit[(u[i]-'0')][(u[i+1]-'0')] = true, visit[(u[i]-'0')][(u[i+1]-'0') + 1] = true;
			if ((u[i+2]-'0') == 2) visit[(u[i]-'0')][(u[i+1]-'0')] = true, visit[(u[i]-'0') + 1][(u[i+1]-'0')] = true, visit[(u[i]-'0') + 2][(u[i+1]-'0')] = true;
			if ((u[i+2]-'0') == 3) visit[(u[i]-'0')][(u[i+1]-'0')] = true, visit[(u[i]-'0')][(u[i+1]-'0') + 1] = true, visit[(u[i]-'0')][(u[i+1]-'0') + 2] = true;
		}
		bool flag = false;
		for (int i = u[u.size() - 3]-'0'+2; i <7; i++){
			if (visit[i][4]){
				flag = true;
				break;
			}
		}
		if (!flag){
			while (u.size()){
				sol.push_back(u);
				u = prev[u];
			}
			reverse(sol.begin(), sol.end());
			solved = 1;
			return;
		}
		else{
			string tem;
			for (int i = 0; i < u.size(); i += 3){
				if ((u[i + 2] - '0') % 4 == 0){
					for (int j = 1;; j++){
						if (!visit[u[i] - '0' + 1+j][u[i + 1] - '0']){
							tem = u;
							tem[i] += j;
							Relax
						}
						else break;
					}
					for (int j = 1;; j++){
						if (!visit[u[i] - '0' - j][u[i + 1] - '0']){
							tem = u;
							tem[i] -= j;
							Relax
						}
						else break;
					}
				}
				if ((u[i + 2] - '0') == 1){
					for (int j = 1;; j++){
						if (!visit[u[i] - '0'][u[i + 1] - '0' + 1+j]){
							tem = u;
							tem[i + 1]+=j;
							Relax
						}
						else break;
					}
					for (int j = 1;; j++){
						if (!visit[u[i] - '0'][u[i + 1] - '0' - j]){
							tem = u;
							tem[i + 1]-=j;
							Relax
						}
						else break;
					}
				}
				if ((u[i + 2] - '0') == 2){
					for (int j = 1;; j++){
						if (!visit[u[i] - '0' + 2+j][u[i + 1] - '0']){
							tem = u;
							tem[i]+=j;
							Relax
						}
						else break;
					}
					for (int j = 1;; j++){
						if (!visit[u[i] - '0' - j][u[i + 1] - '0']){
							tem = u;
							tem[i]-=j;
							Relax
						}
						else break;
					}
				}
				if ((u[i + 2] - '0') == 3){
					for (int j = 1;; j++){
						if (!visit[u[i] - '0'][u[i + 1] - '0' + 2 + j]){
							tem = u;
							tem[i + 1]+=j;
							Relax
						}
						else break;
					}
					for (int j = 1;; j++){
						if (!visit[u[i] - '0'][u[i + 1] - '0' - j]){
							tem = u;
							tem[i + 1]-=j;
							Relax
						}
						else break;
					}
				}
			}
		}
	}
}

void initlvl(int lvl){ 
	v[lvl] = vcpy[lvl];
	moves = 0, movemade = 0, solved = 0, idx=0;
	sol.clear();
	mb = v[lvl].size() - 1;
	memset(vis, true, sizeof(vis));
	for (int i = 1; i < 7; i++){
		for (int j = 1; j < 7; j++){
			vis[i][j] = false;
		}
	}
	vis[7][4] = vis[8][4] = vis[9][4] = false;
	for (int i = 0; i < v[lvl].size(); i++){
		if (v[lvl][i].t % 4 == 0) vis[v[lvl][i].x][v[lvl][i].y] = true, vis[v[lvl][i].x + 1][v[lvl][i].y] = true;
		if (v[lvl][i].t == 1)  vis[v[lvl][i].x][v[lvl][i].y] = true, vis[v[lvl][i].x][v[lvl][i].y + 1] = true;
		if (v[lvl][i].t == 2) vis[v[lvl][i].x][v[lvl][i].y] = true, vis[v[lvl][i].x + 1][v[lvl][i].y] = true, vis[v[lvl][i].x + 2][v[lvl][i].y] = true;
		if (v[lvl][i].t == 3) vis[v[lvl][i].x][v[lvl][i].y] = true, vis[v[lvl][i].x][v[lvl][i].y + 1] = true, vis[v[lvl][i].x][v[lvl][i].y + 2] = true;
	}
}

void iDraw()
{
	iClear();
	iShowBMP(0, 0, bs[st], 0);

	if (st >= 1 && st <= 3){
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 4; j++){
				if (locked[12 * (st - 1) + i * 4 + j]) iShowBMP(175+j*150, 400-i*150, "./Image/lock.bmp", 0);
			}
		}
	}
	if (st == 4){
		iShowBMP(485, 480, ds[lvl / 12], 0);
		if (solved){
			if (idx == sol.size() - 1) idx = 0, solved = 0, sol.clear(), st = 4;
			else{
				movetosolve(idx);
				idx++;
			}
		}
		else{
			DrawBoard();
			if (vis[9][4]){
				moves++;
				if (rmove[lvl] == -1 || moves < rmove[lvl]){
					rmove[lvl] = moves;
					FILE *Fmove = fopen("./Data/MoveData.txt", "w+");
					for (int i = 0; i < 36; i++){
						fprintf(Fmove, "%d ", rmove[i]);
					}
					fclose(Fmove);
				}
				if (lvl == 35){
					st = 7;
				}
				else{
					locked[lvl + 1] = 0;
					FILE *Flvl = fopen("./Data/LockData.txt", "w+");
					for (int i = 0; i < 36; i++){
						fprintf(Flvl, "%d", locked[i] ? 1 : 0);
					}
					fclose(Flvl);
					st = 5;
				}
			}
		}
	}
	if (st == 9){
		if (sound) iShowBMP(250, 300, "./Image/tick.bmp", 0);
		else iShowBMP(450, 300, "./Image/tick.bmp", 0);
	}
}

void iMouseMove(int mx, int my)
{
	if (st == 4){
		int x_ = (mx + 44 - 376) / 64;
		int y_ = (my - 64) / 64;
		if (x_ != v[lvl][mb].x){
			if (x_>v[lvl][mb].x) v[lvl][mb].moveright();
			else v[lvl][mb].moveleft();
		}
		if (y_ != v[lvl][mb].y){
			if (y_>v[lvl][mb].y) v[lvl][mb].moveup();
			else v[lvl][mb].movedown();
		}
	}
}

void iMouse(int button, int state, int mx, int my)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (st == 0){
			if (mx >= 190 && mx <= 290 && my >= 150 && my <= 250){
				st = 8;
			}
			if (mx >= 350 && mx <= 450 && my >= 150 && my <= 250){
				st = 9;
			}
			if (mx >= 510 && mx <= 610 && my >= 150 && my <= 250){
				exit(0);
			}
		}
		if (st >= 1 && st <= 3){
			if (mx < 30 && my>670) st = 0;
			if (mx >= 153 && mx <= 153 + 45 && my >= 542 && my <= 542 + 45) st--, st = (st ? st : 3);
			if (mx >= 603 && mx <= 603 + 45 && my >= 542 && my <= 542 + 45) st++, st = (st == 4 ? 1 : st);
			for (int i = 0; i < 3; i++){
				for (int j = 0; j < 4; j++){
					if (mx >= 125 + 150 * j && mx <= 225 + 150 * j && my >= 400 - 150 * i && my <= 500 - 150 * i){
						if (!locked[12 * (st - 1) + 4 * i + j]){
							lvl = 12 * (st - 1) + 4 * i + j;
							initlvl(lvl);
							st = 4;
						}
					}
				}
			}
		}
		if (st == 4){
			if (mx >= 78 && mx <= 78 + 100 && my >= 360 && my <= 420){
				initlvl(lvl);
			}
			if (mx >= 210 && mx <= 210 + 100 && my >= 360 && my <= 420){
				if (solvetokens) {
					solvetokens--;
					FILE *Ftoken = fopen("./Data/TokenData.txt", "w+");
					fprintf(Ftoken, "%d", solvetokens);
					fclose(Ftoken);
					solve();
				}
				else st = 6;
			}
			if (mx >= 119 && mx <= 119 + 150 && my >= 73 && my <= 73 + 50){
				solved = 0, idx = 0, sol.clear();
				st = 0;
			}
			for (int i = 0; i < v[lvl].size(); i++){
				if (v[lvl][i].t % 4 == 0 && mx + 44 - 376 >= v[lvl][i].x * 64 && mx + 44 - 376 <= (v[lvl][i].x + 2) * 64 && my >= v[lvl][i].y * 64 && my <= (v[lvl][i].y + 1) * 64){
					mb = i;
					movemade = 1;
					break;
				}
				if (v[lvl][i].t == 1 && mx + 44 - 376 >= v[lvl][i].x * 64 && mx + 44 - 376 <= (v[lvl][i].x + 1) * 64 && my >= v[lvl][i].y * 64 && my <= (v[lvl][i].y + 2) * 64){
					mb = i;
					movemade = 1;
					break;
				}
				if (v[lvl][i].t == 2 && mx + 44 - 376 >= v[lvl][i].x * 64 && mx + 44 - 376 <= (v[lvl][i].x + 3) * 64 && my >= v[lvl][i].y * 64 && my <= (v[lvl][i].y + 1) * 64){
					mb = i;
					movemade = 1;
					break;
				}
				if (v[lvl][i].t == 3 && mx + 44 - 376 >= v[lvl][i].x * 64 && mx + 44 - 376 <= (v[lvl][i].x + 1) * 64 && my >= v[lvl][i].y * 64 && my <= (v[lvl][i].y + 3) * 64){
					mb = i;
					movemade = 1;
					break;
				}
			}
			if (movemade) px = v[lvl][mb].x, py = v[lvl][mb].y;
		}
		if (st == 5){
			if (mx >= 250 && mx <= 350 && my >= 150 && my <= 250){
				initlvl(lvl);
				st = 4;
			}
			if (mx >= 450 && mx <= 550 && my >= 150 && my <= 250){
				lvl++;
				initlvl(lvl);
				st = 4;
			}
		}
		if (st == 6){
			if (mx >= 325 && mx <= 525 && my >= 75 && my <= 125){
				st = 4;
			}
		}
		if (st >= 7){
			if (mx >= 325 && mx <= 525 && my >= 75 && my <= 125){
				st = 0;
			}
		}
		if (st == 9){
			if (mx >= 250 && mx <= 350 && my >= 300 && my <= 400){
				sound = 1;
			}
			if (mx >= 450 && mx <= 550 && my >= 300 && my <= 400){
				sound = 0;
			}
		}
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
		if (st == 0){
			if (mx >= 200 && mx <= 600 && my >= 310 && my <= 410){
				st = 1;
			}
		}
		if (st == 4 && movemade){
			if (px != v[lvl][mb].x || py != v[lvl][mb].y){
				moves++;
				if (sound) PlaySound("./Sound/tak.wav", NULL, SND_ASYNC);
			}
			movemade = 0;
		}
	}
}

void iKeyboard(unsigned char key)
{
	
}

void iSpecialKeyboard(unsigned char key)
{

}
int main()
{
	FILE *Flvl = fopen("./Data/LockData.txt", "r");
	char slvl[36];
	fscanf(Flvl, "%s", slvl);
	for (int i = 0; i < 36; i++){
		if (slvl[i] - '0'){
			locked[i] = 1;
		}
		else locked[i] = 0;
	}
	fclose(Flvl);
	FILE *Fmove = fopen("./Data/MoveData.txt", "r");
	int move;
	for (int i = 0; i < 36; i++){
		fscanf(Fmove, "%d", &move);
		rmove[i] = move;
	}
	fclose(Fmove);
	FILE *Ftoken = fopen("./Data/TokenData.txt", "r");
	fscanf(Ftoken, "%d", &solvetokens);
	fclose(Ftoken);
	for (int i = 0; i < 36; i++){
		for (int j = 0; j < 5; j++){
			int n, X, Y;
			fscanf(File, "%d", &n);
			while (n--){
				fscanf(File, "%d %d", &X, &Y);
				block b;
				b.x = X, b.y = Y, b.t = j;
				v[i].push_back(b);
			}
		}
		vcpy[i] = v[i];
	}
	iInitialize(800, 700, "Unblock Me :D");
	return 0;
}