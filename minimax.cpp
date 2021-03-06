// Minimax
#include "AIController.h"
#include <utility>
#include<bits/stdc++.h>
#include <unistd.h>
using namespace std;
#define pp pair<int,int>
#define pp2 pair<int,pp>
#define fi first
#define se second
extern int ai_side;
std::string ai_name = "zhongzero";
const int inf=0x3f3f3f3f;

//init function is called once at the beginning

int H1[9][9],H2[9][9];//H1竖直，H2水平 ；H1 竖直板 放在该点下面 ， H2 水平板 放在该点右边
int H3[9][9];//H3 该点右下角不能被通过
int Blocknum1,Blocknum2;//Blocknum1 我的剩余木板数；Blocknum2 对方的剩余木板数
int X1,Y1,X2,Y2;//(X1,Y1) 我的位置；(X2,Y2) 对方的位置
int cycle;
int Blocknum1_0,Blocknum2_0;
void init() {
	/* Your code here */
	for(int i=0;i<=8;i++)for(int j=0;j<8;j++)H1[i][j]=0;
	for(int i=0;i<8;i++)for(int j=0;j<=8;j++)H2[i][j]=0;
	for(int i=0;i<8;i++)for(int j=0;j<8;j++)H3[i][j]=0;
	Blocknum1=Blocknum2=10;
	if(ai_side==0)X1=8,Y1=4,X2=0,Y2=4;
	else X1=0,Y1=4,X2=8,Y2=4;
	cycle=0;
}

/* The following notation is based on player 0's perspective
 * Rows are labeled 0 through 8 from player 1's side to player 0's side
 * Columns are labeled 0 through 8 from player 0's left to right
 * A coordinate is recorded as the row followed by the column, for example, player 0's pawn starts on (8, 4)
 * A pawn move is recorded as the new coordinate occupied by the pawn
 * A fence placement is recorded as the coordinate of the square whose bottom-right corner fits the center of the wall
 * A typecode is defined as follows: 0: pawn move
 *                                   1: vertical fence placement
 *                                   2: parallel fence placement
 * An action is defined as (typecode, (row-coordinate, column-coordinate))
 * You need to analyze your opponent's action and return your own action
 * If the typecode of your opponent's action is '-1', it means that you are on the offensive.
 */
void update1(pp2 loc){
	if(loc.fi==-1)return;
	if(loc.fi==0)X1=loc.se.fi,Y1=loc.se.se;
	if(loc.fi==1)H1[loc.se.fi][loc.se.se]=H1[loc.se.fi+1][loc.se.se]=1,H3[loc.se.fi][loc.se.se]=1,Blocknum1--;
	if(loc.fi==2)H2[loc.se.fi][loc.se.se]=H2[loc.se.fi][loc.se.se+1]=1,H3[loc.se.fi][loc.se.se]=1,Blocknum1--;
}
void update2(pp2 loc){
	if(loc.fi==-1)return;
	if(loc.fi==0)X2=loc.se.fi,Y2=loc.se.se;
	if(loc.fi==1)H1[loc.se.fi][loc.se.se]=H1[loc.se.fi+1][loc.se.se]=1,H3[loc.se.fi][loc.se.se]=1,Blocknum2--;
	if(loc.fi==2)H2[loc.se.fi][loc.se.se]=H2[loc.se.fi][loc.se.se+1]=1,H3[loc.se.fi][loc.se.se]=1,Blocknum2--;
}
void update1_rev(pp2 loc,int lasx1,int lasy1){
	if(loc.fi==-1)return;
	if(loc.fi==0)X1=lasx1,Y1=lasy1;
	if(loc.fi==1)H1[loc.se.fi][loc.se.se]=H1[loc.se.fi+1][loc.se.se]=0,H3[loc.se.fi][loc.se.se]=0,Blocknum1++;
	if(loc.fi==2)H2[loc.se.fi][loc.se.se]=H2[loc.se.fi][loc.se.se+1]=0,H3[loc.se.fi][loc.se.se]=0,Blocknum1++;
}
void update2_rev(pp2 loc,int lasx2,int lasy2){
	if(loc.fi==-1)return;
	if(loc.fi==0)X2=lasx2,Y2=lasy2;
	if(loc.fi==1)H1[loc.se.fi][loc.se.se]=H1[loc.se.fi+1][loc.se.se]=0,H3[loc.se.fi][loc.se.se]=0,Blocknum2++;
	if(loc.fi==2)H2[loc.se.fi][loc.se.se]=H2[loc.se.fi][loc.se.se+1]=0,H3[loc.se.fi][loc.se.se]=0,Blocknum2++;
}
int isEnd(){//1 I win； -1 opponent win ；0 not end
	if(ai_side==0){
		if(X1==0)return 1;
		if(X2==8)return -1;
	}
	else {
		if(X1==8)return 1;
		if(X2==0)return -1;
	}
	return 0;
}
int fa[90];
int Find(int x){return fa[x]==x?x:fa[x]=Find(fa[x]);}
void merge(int x,int y){
	int g1=Find(x),g2=Find(y);
	if(g1!=g2)fa[g1]=g2;
}
bool isAllBlock(){
	for(int i=0;i<=80;i++)fa[i]=i;
	for(int i=0;i<=8;i++){
		for(int j=0;j<=8;j++){
			int toi=i+1,toj=j;
			if(i!=8&&!H2[i][j])merge(i*9+j,toi*9+toj);
			toi=i,toj=j+1;
			if(j!=8&&!H1[i][j])merge(i*9+j,toi*9+toj);
		}
	}
	int OK=0;
	if(ai_side==0){
		for(int i=0;i<=8;i++)if(Find(i)==Find(X1*9+Y1)){OK++;break;}
		for(int i=72;i<=80;i++)if(Find(i)==Find(X2*9+Y2)){OK++;break;}
	}
	else {
		for(int i=0;i<=8;i++)if(Find(i)==Find(X2*9+Y2)){OK++;break;}
		for(int i=72;i<=80;i++)if(Find(i)==Find(X1*9+Y1)){OK++;break;}
	}
	return OK!=2;
}
double dis[9][9];
bool vis[9][9];
int p[9][9];
queue<pp>W;
// void BFS(int x0,int y0){
// 	for(int i=0;i<9;i++)for(int j=0;j<9;j++)dis[i][j]=inf;
// 	dis[x0][y0]=0;
// 	W.push(pp(x0,y0));
// 	while(!W.empty()){
// 		int x=W.front().fi,y=W.front().se;
// 		W.pop();
// 		if(x-1>=0&&dis[x-1][y]==inf&&!H2[x-1][y])dis[x-1][y]=dis[x][y]+1,W.push(pp(x-1,y));
// 		if(x+1<=8&&dis[x+1][y]==inf&&!H2[x][y])dis[x+1][y]=dis[x][y]+1,W.push(pp(x+1,y));
// 		if(y-1>=0&&dis[x][y-1]==inf&&!H1[x][y-1])dis[x][y-1]=dis[x][y]+1,W.push(pp(x,y-1));
// 		if(y+1<=8&&dis[x][y+1]==inf&&!H1[x][y])dis[x][y+1]=dis[x][y]+1,W.push(pp(x,y+1));
// 	}
// }
void BFS(int x0,int y0){
	for(int i=0;i<9;i++)for(int j=0;j<9;j++)dis[i][j]=inf;
	memset(p,0,sizeof p);
	dis[x0][y0]=0,p[x0][y0]=1;
	W.push(pp(x0,y0));
	while(!W.empty()){
		int x=W.front().fi,y=W.front().se;
		W.pop();
		if(x-1>=0&&!H2[x-1][y]){
			if(dis[x-1][y]==inf)dis[x-1][y]=dis[x][y]+1,W.push(pp(x-1,y));
			if(dis[x-1][y]==dis[x][y]+1)p[x-1][y]+=p[x][y];
		}
		if(x+1<=8&&!H2[x][y]){
			if(dis[x+1][y]==inf)dis[x+1][y]=dis[x][y]+1,W.push(pp(x+1,y));
			if(dis[x+1][y]==dis[x][y]+1)p[x+1][y]+=p[x][y];
		}
		if(y-1>=0&&!H1[x][y-1]){
			if(dis[x][y-1]==inf)dis[x][y-1]=dis[x][y]+1,W.push(pp(x,y-1));
			if(dis[x][y-1]==dis[x][y]+1)p[x][y-1]+=p[x][y];
		}
		if(y+1<=8&&!H1[x][y]){
			if(dis[x][y+1]==inf)dis[x][y+1]=dis[x][y]+1,W.push(pp(x,y+1));
			if(dis[x][y+1]==dis[x][y]+1)p[x][y+1]+=p[x][y];
		}
	}
}
int calc_edge(int x,int y){
	int G=((y==8?1:H1[x][y])+(y==0?1:H1[x][y-1])+(x==8?1:H2[x][y])+(x==0?1:H2[x-1][y]));
	if(G==3)return 0;
	else return G;
	// return min(2,G);
}
// void SPFA(int x0,int y0){
// 	for(int i=0;i<9;i++)for(int j=0;j<9;j++)dis[i][j]=inf;
// 	memset(vis,0,sizeof vis);
// 	dis[x0][y0]=+calc_edge(x0,y0)*0.001;
// 	W.push(pp(x0,y0));
// 	while(!W.empty()){
// 		int x=W.front().fi,y=W.front().se;
// 		W.pop();
// 		vis[x][y]=0;
// 		double val=dis[x][y]+1+calc_edge(x,y)*0.001;
// 		if(x-1>=0&&dis[x-1][y]>val&&!H2[x-1][y]){
// 			dis[x-1][y]=val;
// 			if(!vis[x-1][y])W.push(pp(x-1,y)),vis[x-1][y]=1;
// 		}
// 		if(x+1<=8&&dis[x+1][y]>val&&!H2[x][y]){
// 			dis[x+1][y]=val;
// 			if(!vis[x+1][y])W.push(pp(x+1,y)),vis[x+1][y]=1;
// 		}
// 		if(y-1>=0&&dis[x][y-1]>val&&!H1[x][y-1]){
// 			dis[x][y-1]=val;
// 			if(!vis[x][y-1])W.push(pp(x,y-1)),vis[x][y-1]=1;
// 		}
// 		if(y+1<=8&&dis[x][y+1]>val&&!H1[x][y]){
// 			dis[x][y+1]=val;
// 			if(!vis[x][y+1])W.push(pp(x,y+1)),vis[x][y+1]=1;
// 		}
// 	}
// }
double Dis1,Dis2,Dis3;
int P1,P2;
bool cmpquick;
void GetDis(){
	P1=0,P2=0;
	Dis1=inf,Dis2=inf;
	if(ai_side==0){
		BFS(X1,Y1);
		for(int i=0;i<=8;i++)Dis1=min(Dis1,dis[0][i]);
		for(int i=0;i<=8;i++)if(Dis1==dis[0][i])P1=max(P1,p[0][i]);
		BFS(X2,Y2);
		for(int i=0;i<=8;i++)Dis2=min(Dis2,dis[8][i]);
		for(int i=0;i<=8;i++)if(Dis2==dis[8][i])P2=max(P2,p[8][i]);
		Dis3=dis[X1][Y1];
	}
	else {
		BFS(X1,Y1);
		for(int i=0;i<=8;i++)Dis1=min(Dis1,dis[8][i]);
		for(int i=0;i<=8;i++)if(Dis1==dis[8][i])P1=max(P1,p[8][i]);
		BFS(X2,Y2);
		for(int i=0;i<=8;i++)Dis2=min(Dis2,dis[0][i]);
		for(int i=0;i<=8;i++)if(Dis2==dis[0][i])P2=max(P2,p[0][i]);
		Dis3=dis[X1][Y1];
	}
}
double Getval(bool tp){//evaluate
	
	
	if(isEnd())return isEnd()==1?inf-1:-(inf-1);
	GetDis();
	double val;
	if(tp==1){
		val=(Dis2-Dis1);
		if(Blocknum1_0>=1)val+=Blocknum1*0.5;
		if(Blocknum2_0>=1)val-=Blocknum2*0.5;
		// if(Blocknum1_0>=4&&Blocknum2_0>=4&&Blocknum2_0-Blocknum1_0>=3)val+=Blocknum1*4.0;

		if(Blocknum2_0)val-=calc_edge(X1,Y1)*0.1;
		if(Blocknum1_0)val+=calc_edge(X2,Y2)*0.05;

		if(cycle<=3){
			if(X1<3)val-=abs(X1-3)*10;
			if(X1>5)val-=abs(X1-5)*10;
		}
		if(10-Blocknum1_0<=2){
			if(ai_side==0){
				int num=0;
				// for(int i=X1;i<8;i++)for(int j=0;j<8;j++)num+=H2[i][j];
				for(int j=0;j<8;j++)num+=H2[6][j];
				val+=num*0.3;
			}
			else {
				int num=0;
				// for(int i=0;i<X1;i++)for(int j=0;j<8;j++)num+=H2[i][j];
				for(int j=0;j<8;j++)num+=H2[1][j];
				val+=num*0.3;
			}
		}
		if(cycle<=10)val-=Dis3*0.5;
		if(Blocknum1_0>=3){
			// val+=(-Dis1)*0.3;
			val+=Dis2*0.5;
			// if(cmpquick)val+=(-Dis1)*0.3;
			// else val+=Dis2*0.3;
		}
	}
	else {
		// val=Dis2-Dis1;
	}
	return val;
}
double MiniMaxSearch(int tp,int dep,const double &FaMinMax,pp2 &ret){//tp=0 我的回合；tp=1 对方的回合
	// double NowVal=Getval(tp);
	// if(NowVal==LasVal)return tp==1?-inf:inf;
	int lasx1=X1,lasy1=Y1,lasx2=X2,lasy2=Y2;
	pp2 G,ans,tmpans;
	double Val;
	if(dep==0||isEnd()){
		Val=Getval(tp);
		// cerr<<"!!! "<<X1<<" "<<Y1<<" "<<X2<<" "<<Y2<<" "<<Val<<endl;
		return Val;
	}
	if(tp==0){
		Val=-inf;

		// 0：move
		G.fi=0;
		int tox1,toy1;
		tox1=X1+1,toy1=Y1;
		if(! (tox1<0||tox1>8||toy1<0||toy1>8||H2[X1][Y1]  ) ){
			if(tox1==X2&&toy1==Y2){
				tox1=X1+2,toy1=Y1;
				if(! (tox1<0||tox1>8||toy1<0||toy1>8||H2[X1+1][Y1]  ) ){
					G.se=pp(tox1,toy1);
					update1(G);
					double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
					if(tmp>Val)Val=tmp,ans=G;
					update1_rev(G,lasx1,lasy1);
					if(Val>=FaMinMax){ret=ans;return Val;}
				}
				else {
					tox1=X1+1,toy1=Y1-1;
					if(! (tox1<0||tox1>8||toy1<0||toy1>8||H1[X1+1][Y1-1]  ) ){
						G.se=pp(tox1,toy1);
						update1(G);
						double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
						if(tmp>Val)Val=tmp,ans=G;
						update1_rev(G,lasx1,lasy1);
						if(Val>=FaMinMax){ret=ans;return Val;}
					}

					tox1=X1+1,toy1=Y1+1;
					if(! (tox1<0||tox1>8||toy1<0||toy1>8||H1[X1+1][Y1]  ) ){
						G.se=pp(tox1,toy1);
						update1(G);
						double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
						if(tmp>Val)Val=tmp,ans=G;
						update1_rev(G,lasx1,lasy1);
						if(Val>=FaMinMax){ret=ans;return Val;}
					}
				}
			}
			else{
				G.se=pp(tox1,toy1);
				update1(G);
				double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
				if(tmp>Val)Val=tmp,ans=G;
				update1_rev(G,lasx1,lasy1);
				if(Val>=FaMinMax){ret=ans;return Val;}
			}
		}
		tox1=X1-1,toy1=Y1;
		if(! (tox1<0||tox1>8||toy1<0||toy1>8||H2[X1-1][Y1]  ) ){
			if(tox1==X2&&toy1==Y2){
				tox1=X1-2,toy1=Y1;
				if(! (tox1<0||tox1>8||toy1<0||toy1>8||H2[X1-2][Y1]  ) ){
					G.se=pp(tox1,toy1);
					update1(G);
					double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
					if(tmp>Val)Val=tmp,ans=G;
					update1_rev(G,lasx1,lasy1);
					if(Val>=FaMinMax){ret=ans;return Val;}
				}
				else {
					tox1=X1-1,toy1=Y1-1;
					if(! (tox1<0||tox1>8||toy1<0||toy1>8||H1[X1-1][Y1-1]  ) ){
						G.se=pp(tox1,toy1);
						update1(G);
						double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
						if(tmp>Val)Val=tmp,ans=G;
						update1_rev(G,lasx1,lasy1);
						if(Val>=FaMinMax){ret=ans;return Val;}
					}

					tox1=X1-1,toy1=Y1+1;
					if(! (tox1<0||tox1>8||toy1<0||toy1>8||H1[X1-1][Y1]  ) ){
						G.se=pp(tox1,toy1);
						update1(G);
						double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
						if(tmp>Val)Val=tmp,ans=G;
						update1_rev(G,lasx1,lasy1);
						if(Val>=FaMinMax){ret=ans;return Val;}
					}
				}
			}
			else{
				G.se=pp(tox1,toy1);
				update1(G);
				double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
				if(tmp>Val)Val=tmp,ans=G;
				update1_rev(G,lasx1,lasy1);
				if(Val>=FaMinMax){ret=ans;return Val;}
			}
		}
		tox1=X1,toy1=Y1+1;
		if(! (tox1<0||tox1>8||toy1<0||toy1>8||H1[X1][Y1]  ) ){
			if(tox1==X2&&toy1==Y2){
				tox1=X1,toy1=Y1+2;
				if(! (tox1<0||tox1>8||toy1<0||toy1>8||H1[X1][Y1+1]  ) ){
					G.se=pp(tox1,toy1);
					update1(G);
					double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
					if(tmp>Val)Val=tmp,ans=G;
					update1_rev(G,lasx1,lasy1);
					if(Val>=FaMinMax){ret=ans;return Val;}
				}
				else {
					tox1=X1-1,toy1=Y1+1;
					if(! (tox1<0||tox1>8||toy1<0||toy1>8||H2[X1-1][Y1+1]  ) ){
						G.se=pp(tox1,toy1);
						update1(G);
						double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
						if(tmp>Val)Val=tmp,ans=G;
						update1_rev(G,lasx1,lasy1);
						if(Val>=FaMinMax){ret=ans;return Val;}
					}

					tox1=X1+1,toy1=Y1+1;
					if(! (tox1<0||tox1>8||toy1<0||toy1>8||H2[X1][Y1+1]  ) ){
						G.se=pp(tox1,toy1);
						update1(G);
						double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
						if(tmp>Val)Val=tmp,ans=G;
						update1_rev(G,lasx1,lasy1);
						if(Val>=FaMinMax){ret=ans;return Val;}
					}
				}
			}
			else{
				G.se=pp(tox1,toy1);
				update1(G);
				double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
				if(tmp>Val)Val=tmp,ans=G;
				update1_rev(G,lasx1,lasy1);
				if(Val>=FaMinMax){ret=ans;return Val;}
			}
		}
		tox1=X1,toy1=Y1-1;
		if(! (tox1<0||tox1>8||toy1<0||toy1>8||H1[X1][Y1-1]  ) ){
			if(tox1==X2&&toy1==Y2){
				tox1=X1,toy1=Y1-2;
				if(! (tox1<0||tox1>8||toy1<0||toy1>8||H1[X1][Y1-2]  ) ){
					G.se=pp(tox1,toy1);
					update1(G);
					double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
					if(tmp>Val)Val=tmp,ans=G;
					update1_rev(G,lasx1,lasy1);
					if(Val>=FaMinMax){ret=ans;return Val;}
				}
				else {
					tox1=X1-1,toy1=Y1-1;
					if(! (tox1<0||tox1>8||toy1<0||toy1>8||H2[X1-1][Y1-1]  ) ){
						G.se=pp(tox1,toy1);
						update1(G);
						double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
						if(tmp>Val)Val=tmp,ans=G;
						update1_rev(G,lasx1,lasy1);
						if(Val>=FaMinMax){ret=ans;return Val;}
					}

					tox1=X1+1,toy1=Y1-1;
					
					if(! (tox1<0||tox1>8||toy1<0||toy1>8||H2[X1][Y1-1]  ) ){
						G.se=pp(tox1,toy1);
						update1(G);
						double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
						if(tmp>Val)Val=tmp,ans=G;
						update1_rev(G,lasx1,lasy1);
						if(Val>=FaMinMax){ret=ans;return Val;}
					}
				}
			}
			else{
				G.se=pp(tox1,toy1);
				update1(G);
				double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
				if(tmp>Val)Val=tmp,ans=G;
				update1_rev(G,lasx1,lasy1);
				if(Val>=FaMinMax){ret=ans;return Val;}
			}
		}

		if(Blocknum1){
			// 1：verical block
			G.fi=1;
			for(int i=0;i<8;i++){
				for(int j=0;j<8;j++){
					if(H1[i][j]||H1[i+1][j]||H3[i][j])continue;
					G.se=pp(i,j);
					update1(G);
					if(!isAllBlock()){
						double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
						if(tmp>Val)Val=tmp,ans=G;
					}
					update1_rev(G,lasx1,lasy1);
					if(Val>=FaMinMax){ret=ans;return Val;}
				}
			}
			// 2：horizontal block
			G.fi=2;
			for(int i=0;i<8;i++){
				for(int j=0;j<8;j++){
					if(H2[i][j]||H2[i][j+1]||H3[i][j])continue;
					G.se=pp(i,j);
					update1(G);
					if(!isAllBlock()){
						double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
						if(tmp>Val)Val=tmp,ans=G;
					}
					update1_rev(G,lasx1,lasy1);
					if(Val>=FaMinMax){ret=ans;return Val;}
				}
			}
		}
	}
	else {
		Val=inf;
		// 0：move
		G.fi=0;
		int tox2,toy2;
		tox2=X2+1,toy2=Y2;
		if(! (tox2<0||tox2>8||toy2<0||toy2>8||H2[X2][Y2]  ) ){
			if(tox2==X1&&toy2==Y1){
				tox2=X2+2,toy2=Y2;
				if(! (tox2<0||tox2>8||toy2<0||toy2>8||H2[X2+1][Y2]  ) ){
					G.se=pp(tox2,toy2);
					update2(G);
					double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
					if(tmp<Val)Val=tmp,ans=G;
					update2_rev(G,lasx2,lasy2);
					if(Val<=FaMinMax){ret=ans;return Val;}
				}
				else {
					tox2=X2+1,toy2=Y2-1;
					if(! (tox2<0||tox2>8||toy2<0||toy2>8||H1[X2+1][Y2-1]  ) ){
						G.se=pp(tox2,toy2);
						update2(G);
						double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
						if(tmp<Val)Val=tmp,ans=G;
						update2_rev(G,lasx2,lasy2);
						if(Val<=FaMinMax){ret=ans;return Val;}
					}

					tox2=X2+1,toy2=Y2+1;
					
					if(! (tox2<0||tox2>8||toy2<0||toy2>8||H1[X2+1][Y2]  ) ){
						G.se=pp(tox2,toy2);
						update2(G);
						double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
						if(tmp<Val)Val=tmp,ans=G;
						update2_rev(G,lasx2,lasy2);
						if(Val<=FaMinMax){ret=ans;return Val;}
					}
				}
			}
			else{
				G.se=pp(tox2,toy2);
				update2(G);
				double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
				if(tmp<Val)Val=tmp,ans=G;
				update2_rev(G,lasx2,lasy2);
				if(Val<=FaMinMax){ret=ans;return Val;}
			}
		}
		tox2=X2-1,toy2=Y2;
		if(! (tox2<0||tox2>8||toy2<0||toy2>8||H2[X2-1][Y2]  ) ){
			if(tox2==X1&&toy2==Y1){
				tox2=X2-2,toy2=Y2;
				if(! (tox2<0||tox2>8||toy2<0||toy2>8||H2[X2-2][Y2]  ) ){
					G.se=pp(tox2,toy2);
					update2(G);
					double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
					if(tmp<Val)Val=tmp,ans=G;
					update2_rev(G,lasx2,lasy2);
					if(Val<=FaMinMax){ret=ans;return Val;}
				}
				else {
					tox2=X2-1,toy2=Y2-1;
					if(! (tox2<0||tox2>8||toy2<0||toy2>8||H1[X2-1][Y2-1]  ) ){
						G.se=pp(tox2,toy2);
						update2(G);
						double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
						if(tmp<Val)Val=tmp,ans=G;
						update2_rev(G,lasx2,lasy2);
						if(Val<=FaMinMax){ret=ans;return Val;}
					}

					tox2=X2-1,toy2=Y2+1;
					
					if(! (tox2<0||tox2>8||toy2<0||toy2>8||H1[X2-1][Y2]  ) ){
						G.se=pp(tox2,toy2);
						update2(G);
						double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
						if(tmp<Val)Val=tmp,ans=G;
						update2_rev(G,lasx2,lasy2);
						if(Val<=FaMinMax){ret=ans;return Val;}
					}
				}
			}
			else{
				G.se=pp(tox2,toy2);
				update2(G);
				double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
				if(tmp<Val)Val=tmp,ans=G;
				update2_rev(G,lasx2,lasy2);
				if(Val<=FaMinMax){ret=ans;return Val;}
			}
		}
		tox2=X2,toy2=Y2+1;
		if(! (tox2<0||tox2>8||toy2<0||toy2>8||H1[X2][Y2]  ) ){
			if(tox2==X1&&toy2==Y1){
				tox2=X2,toy2=Y2+2;
				if(! (tox2<0||tox2>8||toy2<0||toy2>8||H1[X2][Y2+1]  ) ){
					G.se=pp(tox2,toy2);
					update2(G);
					double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
					if(tmp<Val)Val=tmp,ans=G;
					update2_rev(G,lasx2,lasy2);
					if(Val<=FaMinMax){ret=ans;return Val;}
				}
				else {
					tox2=X2-1,toy2=Y2+1;
					if(! (tox2<0||tox2>8||toy2<0||toy2>8||H2[X2-1][Y2+1]  ) ){
						G.se=pp(tox2,toy2);
						update2(G);
						double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
						if(tmp<Val)Val=tmp,ans=G;
						update2_rev(G,lasx2,lasy2);
						if(Val<=FaMinMax){ret=ans;return Val;}
					}

					tox2=X2+1,toy2=Y2+1;
					
					if(! (tox2<0||tox2>8||toy2<0||toy2>8||H2[X2][Y2+1]  ) ){
						G.se=pp(tox2,toy2);
						update2(G);
						double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
						if(tmp<Val)Val=tmp,ans=G;
						update2_rev(G,lasx2,lasy2);
						if(Val<=FaMinMax){ret=ans;return Val;}
					}
				}
			}
			else{
				G.se=pp(tox2,toy2);
				update2(G);
				double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
				if(tmp<Val)Val=tmp,ans=G;
				update2_rev(G,lasx2,lasy2);
				if(Val<=FaMinMax){ret=ans;return Val;}
			}
		}
		tox2=X2,toy2=Y2-1;
		if(! (tox2<0||tox2>8||toy2<0||toy2>8||H1[X2][Y2-1]  ) ){
			if(tox2==X1&&toy2==Y1){
				tox2=X2,toy2=Y2-2;
				if(! (tox2<0||tox2>8||toy2<0||toy2>8||H1[X2][Y2-2]  ) ){
					G.se=pp(tox2,toy2);
					update2(G);
					double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
					if(tmp<Val)Val=tmp,ans=G;
					update2_rev(G,lasx2,lasy2);
					if(Val<=FaMinMax){ret=ans;return Val;}
				}
				else {
					tox2=X2-1,toy2=Y2-1;
					if(! (tox2<0||tox2>8||toy2<0||toy2>8||H2[X2-1][Y2-1]  ) ){
						G.se=pp(tox2,toy2);
						update2(G);
						double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
						if(tmp<Val)Val=tmp,ans=G;
						update2_rev(G,lasx2,lasy2);
						if(Val<=FaMinMax){ret=ans;return Val;}
					}

					tox2=X2+1,toy2=Y2-1;
					
					if(! (tox2<0||tox2>8||toy2<0||toy2>8||H2[X2][Y2-1]  ) ){
						G.se=pp(tox2,toy2);
						update2(G);
						double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
						if(tmp<Val)Val=tmp,ans=G;
						update2_rev(G,lasx2,lasy2);
						if(Val<=FaMinMax){ret=ans;return Val;}
					}
				}
			}
			else{
				G.se=pp(tox2,toy2);
				update2(G);
				double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
				if(tmp<Val)Val=tmp,ans=G;
				update2_rev(G,lasx2,lasy2);
				if(Val<=FaMinMax){ret=ans;return Val;}
			}
		}

		if(Blocknum2){
			// 1：verical block
			G.fi=1;
			for(int i=0;i<8;i++){
				for(int j=0;j<8;j++){
					if(H1[i][j]||H1[i+1][j]||H3[i][j])continue;
					G.se=pp(i,j);
					update2(G);
					if(!isAllBlock()){
						double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
						if(tmp<Val)Val=tmp,ans=G;
					}
					update2_rev(G,lasx2,lasy2);
					if(Val<=FaMinMax){ret=ans;return Val;}
				}
			}
			// 2：horizontal block
			G.fi=2;
			for(int i=0;i<8;i++){
				for(int j=0;j<8;j++){
					if(H2[i][j]||H2[i][j+1]||H3[i][j])continue;
					G.se=pp(i,j);
					update2(G);
					if(!isAllBlock()){
						double tmp=MiniMaxSearch(tp^1,dep-1,Val,tmpans);
						if(tmp<Val)Val=tmp,ans=G;
					}
					update2_rev(G,lasx2,lasy2);
					if(Val<=FaMinMax){ret=ans;return Val;}
				}
			}
		}
	}
	ret=ans;
	return Val;
}
pp2 action(pp2 loc) {
	cycle++;
	update2(loc);

	pp2 ans;

	// bool flag=0;
	// if(cycle==1&&ai_side==0){
	// 	ans=pp2(0,pp(8,5)),flag=1;
	// }
	// if(flag){
	// 	update1(ans);
	// 	return ans;
	// }

	GetDis();
	cmpquick=(Dis1<Dis2?1:0);
	Blocknum1_0=Blocknum1,Blocknum2_0=Blocknum2;

	
	MiniMaxSearch(0,Blocknum1?3:1,inf+1,ans);
	// cerr<<"!!!"<<ans.fi<<" "<<ans.se.fi<<" "<<ans.se.se<<endl;
	update1(ans);
	// usleep(300000);
	return ans;
}