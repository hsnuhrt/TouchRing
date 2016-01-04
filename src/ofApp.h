#pragma once

#include "ofMain.h"
#include <time.h>
#include <stdio.h>      /* printf */
#include <iostream>
#include <math.h>       /* asin */
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <windows.h>
#include <vector>
#include <sstream>


//#include "ofxGui.h"
using namespace std;
using namespace cv;

class ofApp : public ofBaseApp{
	
	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void clearbuf(void);
		void readbuf(int result);
		void readbtob(void);
		void cleardata(void);
		void clearelet(int x);			//0: elec, 1:touch, 2: both, 3: elecgs, 4 elecgc 
		void captoelec(int cap, int no, int sf);	// oo|o
		void captoelecr1(int cap, int no, int sf);	// oo|o
		void touchrecog(void);
		//void touchrecogn(void);
		void checknb(int row, int col, int flag, float & rwn, int & nbn);	//flag: 0. electrodes, 1. touch, return neighbor number
		void gesrecog(void);			//Touch:	MT t: 1xx, MT u: 2xx, TT t: 3xx
										//TG x1x:	111: tTGup, 211: uTGup, 112: tTGdown, 212: uTGdown, 113: tTGleft, 213: uTGleft
										//			114: tTGright, 214: uTGright, 115: tTGtap, 215: uTGtap
										//MG x2x:	321: tMGup, 322: tMGdown

		void gesrecogg(void);
		float dist(vector<int> a, vector <int> b);
		float distf(vector<float> a, vector <float> b);

		void exrecog(int ges);			//experiment for recognition
		void exrecog1(int ges);
		void exre_init(void);
        string exreinttostr(int ges);
        void build_gesture_type(void);


		void list1d(int act);			//touch screen: up, down, cursor: up, down, tap
		void icon2d(int act);			//touch screen: left, right, cursor: up, down left, right, tap
		void icon2d_init(void);
		
		void zoom(int act);				//touch screen: up, down, left, right, cursor: up, down left, right, tap, zoom in, zoom out

		void delayt(int ms);
		void testele(void);
		void initvec(void);

		ofTrueTypeFont		font;
		ofTrueTypeFont		fontL;
		
		static const int datalen = 5, bufflen = 120, eleccn = 3, elecrn = 12, mtn = 3, consecnum = 2, dir = 4;
		unsigned char		buffer[bufflen];			//90 for all 85
		char				data[datalen];
		int					elec[elecrn][eleccn];		//ex: (11, 2)
		int					elecgs[elecrn][eleccn];
		int					elecgc[elecrn][eleccn];
		int					capnum;
		vector<vector<vector<int>>>	touchlog;		//touchlog[vl][mtn][r and c];		//(thumb, middle finger,) or (,,back)
		vector<vector<int>>	elecnlog;				//touchlog[vl][mtn]
		vector<int> logun;							//untouched (initial)
		vector<float> logunf;
		int					vl;						//touchlog length
		int					touch[elecrn][eleccn];
		int					gesture, gesture1;				//0: none,
		int					action;					//0: none, 1: up, 2: down, 3: left, 4: right, 5: tap
		int					mode;					//mode of experiment: 0. none, 1. recognition experiment, 2. 1D list, 3. 3D icons, 4. zooming, 5. 3D exploration
		int					trow, tcol;
		float				maxdist, maxdistf, maxdistff, maxdistffx;			
		int					move; 
		int					touched[mtn];
		int					csecc[mtn][consecnum], cseccf[mtn], cseccfx[mtn];
		int					vote[dir];
		int					drawflag;
		int					capflag;
		int					iniflag;
		int					nonlift;
		int					gflag, gcount,grecflag;
		int					trainflag, tcount, tswipe;
		float				tcf, trf, tmaxr, tstr, tmaxc, tstc, tmtn; 

		int	thnbn;
		int ininoise;
		int touthf;
		int mideln;
		int mdftf, mdftftap;
		float uprth, downrth, leftcth, rightcth, tapth;


		vector<vector<vector<int>>> csecv;			//csecv[mtn][consecnum][r and c];
		clock_t touchst[mtn];

		vector<int>	gesst, gesend, gesmaxd, gesstfx, gesmaxfx;							//gesture start/end (r, c) 
		vector<float> thumbavg, ms, mmax,  gesstf, gesmaxf;
		vector<vector<vector<float>>> gesavg;

		static const int	exretimes = 15;
		vector<pair<string,int>>   exre_array;
        vector<pair<string,int>>   gesturetype;
		bool				writeflag;
        int                 exrenow, exrecolor;
		int					exrecorrect, exreflag, exrehit, exreges;
		int					exrecorrect1, exreflag1, exrehit1, exreges1;
        int                 iconheight, iconwidth;
        int                 cursorX, cursorY, targetX, targetY;
        int                 iconcorrect, iconhit;


		ofSerial	serial;
};

