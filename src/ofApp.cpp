#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	
	ofBackground(255);	
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	font.loadFont("DIN.otf", 20);
	fontL.loadFont("DIN.otf", 40);
	
	serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
	
	int baud = 9600;
	serial.setup("COM7", baud); //open the first device
	capnum = -1;
	mode = 0;
	//exre_init();
	clearelet(2);
	clearelet(3);
	clearelet(4);
	clearelet(5);
	vl = 50;
	logun.resize(2, -1);
	logunf.resize(2, -1);
	thumbavg.resize(2, -1);
	gesstf = logunf;
	gesstfx = logun;
	initvec();
	maxdist = 0, maxdistf = 0, maxdistff = 0, maxdistffx = 0;
	move = -1;
	gesture = 0, gesture1 = 0;
	gflag = 0, gcount = 0, grecflag = 0;
	trainflag = 0, tcount = 0, tswipe = 0;  //swipe: 1: up, 2: down, 3: left, 4: right
	drawflag = 0;
	capflag = 0;
	iniflag = 0;
	tcf = 0, trf = 0, tmaxr = 0, tstr = 0, tmaxc = 0, tstc = 0, tmtn = 0; 
	for(int i = 0; i < mtn; i++){
		for(int j = 0; j < consecnum; j++){
			csecc[i][j] = 0; 
		}
		touched[i] = 0;
		cseccf[i] = 0;
		cseccfx[i] = 0;
	}
	build_gesture_type();


///////////////////////////////initial parameter////////////////////////////////////
	thnbn = 3;						//neighbor number threshold

	ininoise = 20;					//initial noise elimitation
	touthf = 2;						//consective touch threshold
	mideln = 0;						//middle finger electrode number
	mdftf = 10,  mdftftap = 5;						//middle finger touch threshold
	tapth = 0.5;
	uprth = 2.2;
	downrth = 2.2;
	leftcth = 1.2;
	rightcth = 1.2;
}

//--------------------------------------------------------------
void ofApp::update(){
	clearbuf();
	int br = serial.readBytes( &buffer[0], bufflen);
	capflag = 0;
	readbuf(br);
	//readbtob();
	//testele();

	//for(int i = 0; i < 12; i++){
	//	for(int j = 0; j < 3; j++){
	//		if(elec[i][j] == 1){
	//			cout<<i<<j<<"\t";
	//		}
	//	}
	//	//cout<<endl;
	//}
	//cout<<endl<<endl;

	if(br > 0){
		if(mode == 0){
			if(gflag == 1){
				touchrecog();
				/*cout<<endl<<"cin r: "<<endl;
				cin>>touchlog[vl-1][0][0];
				cout<<"cin c: "<<endl;
				cin>>touchlog[vl-1][0][1];
				*/

				/*for(int i = vl-2; i < vl; i++){
					cout<<endl;
					for(int j = 0; j < 3; j++){
						cout<<touchlog[i][j][0]<<" ";
						cout<<touchlog[i][j][1]<<"\t";
					}
				}
				cout<<endl;*/
	
				/*for(int i = vl-2; i < vl; i++){
					cout<<touchlog[i][0][0]<<" ";
					cout<<touchlog[i][0][1]<<"\n";
				}*/

	
				/*gesrecog();
				if(gesture != 0){
					cout<<endl<<"gesture: "<<exreinttostr(gesture)<<endl;
				}*/
				//if(gesture != 0){
				//	cout<<endl<<"gesture: "<<exreinttostr(gesture)<<endl;
				//	/*for(int i = vl-2; i < vl; i++){
				//		cout<<touchlog[i][0][0]<<" ";
				//		cout<<touchlog[i][0][1]<<"\n";
				//	}*/
				//}
				gcount++;
			}
			else if(gflag == 0){
				if(grecflag == 1){
					gesrecogg();
					if(gesture != 0){
						cout<<endl<<"gesture: "<<exreinttostr(gesture)<<endl;
						//cout<<endl<<"gesture1: "<<exreinttostr(gesture1)<<endl;
						cout<<"gcount: "<<gcount<<endl<<endl;
					}
					gcount = 0;
					grecflag = 0;
				}

			}
		}
		else if(mode == 1){
			if(drawflag == 1){
				delayt(200);
				drawflag = 0;
			}

			if(gflag == 1){
					touchrecog();
					gcount++;
					//cout<<gcount<<endl;
			}
			else if(gflag == 0){
			}
		}
	}
	/*else{
		cout<<"zzz\n";
	}*/
	delayt(50);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(0);

	//check electrodes
	string msg;
	msg += "Electrodes:\n";
	msg += "\nThumb:\n";
	for(int i = 0; i < elecrn; i++){
		if(i == 6){
			msg += "\nMiddle finger:\n";
		}
		else if(i == 9){
			msg += "\nBack:\n";
		}
		for(int j = 0; j < eleccn; j++){
			msg += ofToString(elec[i][j])+"\t";
		}
		msg += "\n";
	}

	/*for(int i = 0; i < elecrn; i++){
		for(int j = 0; j < eleccn; j++){
			msg += ofToString(touch[i][j])+"\t";
		}
		msg += "\n";
	}*/
	/*if(gesture != 0){
		msg += "\ngesture: "+ ofToString(gesture)+"\n";
	}*/

	font.drawString(msg, 50, 100);


	
	if(mode == 0){
		string msg1;
		if(gflag == 1){
			if(gcount >= ininoise){
				msg1 += "Start";
				if(iniflag == 1){
					Beep(1400, 200);
					iniflag = 2;
				}
			}
			else{
				msg1 +="Initial";
			}
		}
		else{
			msg1 += "Ready";
		}
		//ofSetColor(0);
		fontL.drawString(msg1, 300, 300);
    }
    else if(mode == 1){
        switch (exrecolor){
            case 1:
                ofSetColor(0,255,0);
				drawflag = 1;
                break;
            case 2:
                ofSetColor(255,0,0);
				drawflag = 1;
                break;
            default:
                ofSetColor(0, 0, 0);
                break;
        }

		fontL.drawString(ofToString(exrenow) +"\n"+ exre_array[exrenow].first, 300, 400);
 

		if (exrecolor){
			//delayt(1000);
			exrecolor = 0;
			exrenow++;
			writeflag = true;
		}

		string msg1;
		if(gflag == 1){
			if(gcount > ininoise){
				msg1 += "Start";
				if(iniflag == 1){
					Beep(1400, 200);
					iniflag = 2;
				}
			}
			else{ 
				msg1 +="Initial";
			}
		}
		else{
			msg1 += "Ready";
		}
		//ofSetColor(0);
		fontL.drawString(msg1, 300, 300);
    }
    else if(mode == 2){
        ofSetColor(0,0,0);
        for (int i = 0; i < iconheight; ++i){
            for (int j = 0; j < iconwidth; ++j){
                ofRect(400+60*j, 200+60*i, 52, 52);
            }
        }
        ofSetColor(0,255,0);
        ofRect(400+60*cursorX, 200+60*cursorY, 52, 52);
        ofSetColor(255,0,0);
        ofRect(400+60*targetX, 200+60*targetY, 52, 52);
        
        if (iconhit == 1)
            fontL.drawString("GOOD", 200, 100);
        else
            fontL.drawString("BAD", 200, 100);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
	if (mode == 0){
        if (key == 'm'){
            mode = 1;
            exre_init();
        }
		if (key == ' '){
			if(gflag == 0){
				gflag = 1;
			}
			else if(gflag == 1){
				gflag = 0;
				grecflag = 1;
			}
		}
		if (key == 't'){
			if(trainflag == 0){	
				trainflag = 1;
				cout<<"Enter swipe direction: ";
				cin>>tswipe;
				cout<<endl;
			}
			else{ 
				trainflag = 0;
				tcf = 0, trf = 0, tmaxr = 0, tstr = 0, tmaxc = 0, tstc = 0, tmtn = 0;
				cout<<"Training done\n";
				tswipe = 0;
				tcount = 0;
			}
		}
    }
    else if (mode == 1){
        if (key == ' '){
			if(gflag == 0){
				gflag = 1;
			}
			else if(gflag == 1){
				gflag = 0;
				grecflag = 1;
				if(grecflag == 1){
					gesrecogg();
					if(gesture != 0){
						cout<<endl<<"gesture: "<<exreinttostr(gesture)<<endl;
						//cout<<endl<<"gesture1: "<<exreinttostr(gesture1)<<endl;
						cout<<"gcount: "<<gcount<<endl<<endl;
					}
					exrecog(gesture);
					if (exrenow < exre_array.size()-1){
						if (exreflag == 0){
							exreflag = 1;
						}
						else {
							exrehit = 0;
						}

						/*if (exreflag1 == 0){
							exreflag1 = 1;
						}
						else {
							exrehit1 = 0;
						}*/
            
						if (exrenow > 0 && writeflag){
							fstream fp;
							fp.open("exrelog.txt", ios::out|ios::app);
							fp << exreinttostr(exre_array[exrenow].second) << "\t" << exrehit << "\t" << exreinttostr(exreges) << endl;
							fp.close();
							//fp.open("exrelog1.txt", ios::out|ios::app);
							//fp << exreinttostr(exre_array[exrenow].second) << "\t" << exrehit1 << "\t" << exreinttostr(exreges1) << endl;
							//fp.close();
							writeflag = false;
						}
            
						if (exrehit)
							exrecolor = 1;
						else
							exrecolor = 2;

						exreges = 0;
					}
					gcount = 0;
					grecflag = 0;
				}
			}
            
        }
        else if (key == 'm'){
            mode = 2;
            icon2d_init();
        }
    }
    else if (mode == 2){
        if (key == OF_KEY_LEFT){
            if (cursorX > 0)
                cursorX--;
        }
        else if (key == OF_KEY_UP){
            if (cursorY > 0)
                cursorY--;
        }
        else if (key == OF_KEY_RIGHT){
            if (cursorX < iconwidth-1)
                cursorX++;
        }
        else if (key == OF_KEY_DOWN){
            if (cursorY < iconheight-1)
                cursorY++;
        }
        else if (key == ' '){
            icon2d(1);
        }
        else if (key == 'm'){
            mode = 0;
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
	
}

void ofApp::readbuf(int result){
	if ( result == OF_SERIAL_NO_DATA ){
		printf("no data was read");
	}
	else if ( result == OF_SERIAL_ERROR ){
		printf("an error occurred");
	}
	else{
		int k;
		int df = 0;
		capnum = -1;
		if(result > 0){
			clearelet(2);
		}
		for(int j = 0; j < result; j++){
			switch(buffer[j]){
				case 'A':{
					df = 1;
					k = 0;
					break;
				}
				case 'B':{
					df = 2;
					k = 0;
					break;
				}
				case 'C':{
					df = 3;
					k = 0;
					break;
				}
				case 'D':{
					df = 4;
					k = 0;
					break;
				}
				case 'E':{
					df = 5;
					k = 0;
					break;
				}
				case 'X':{
					df = 6;
					k = 0;
					break;
				}
				case '\t':{
					if(df != 0 && df != 6){
						int no = atof (data);
						int cap = df;
						captoelecr1(cap, no, 0);
						capflag = 1;
						df = 0;
						k = 0;
						//cout<<cap<<no<<"\t";
						cleardata();
					}
					if(df != 0 && df == 6){
						capnum = atof (data);
						//cout<<"capnum: "<<capnum<<endl;
						df = 0;
						k = 0;
						cleardata();
					}
					break;
				}
				default:{ 
					if(df != 0){
						data[k] = (char)buffer[j];
						k++;
					}
				}
			}
		}
		/*for(int i = 0; i < result; i++){
			cout<<buffer[i];
		}
		cout<<endl;*/
		//cout<<endl<<endl;
	}
}

void ofApp::readbtob(void){
	char myByte = 0;
	int df = 0;
	int k;
	capnum = -1;
	clearelet(2);
	do{
		myByte = serial.readByte();
		if ( myByte == OF_SERIAL_NO_DATA ){
			printf("no data was read");
		}
		else if ( myByte == OF_SERIAL_ERROR ){
			printf("an error occurred");
		}
		else{
			cout<<myByte;
			
			switch(myByte){
				case 'A':{
					df = 1;
					k = 0;
					break;
				}
				case 'B':{
					df = 2;
					k = 0;
					break;
				}
				case 'C':{
					df = 3;
					k = 0;
					break;
				}
				case 'D':{
					df = 4;
					k = 0;
					break;
				}
				case 'E':{
					df = 5;
					k = 0;
					break;
				}
				case 'X':{
					df = 6;
					k = 0;
					break;
				}
				case '\t':{
					if(df != 0 && df != 6){
						int no = atof (data);
						int cap = df;
						captoelec(cap, no, 0);
						df = 0;
						k = 0;
						cout<<cap<<no<<"\t";
						cleardata();
					}
					if(df != 0 && df == 6){
						capnum = atof (data);
						//cout<<"capnum: "<<capnum<<endl;
						df = 0;
						k = 0;
						cleardata();
					}
					break;
				}
				default:{ 
					if(df != 0){
						data[k] = (char)myByte;
						k++;
					}
				}
			}
			
			/*for(int i = 0; i < result; i++){
				cout<<buffer[i];
			}
			cout<<endl;*/
			cout<<endl<<endl;
		}
	}while(myByte == ',');
}

void ofApp::clearbuf(void){
	for(int i = 0; i < bufflen; i++){
		buffer[i] = 0;
	}
}

void ofApp::cleardata(void){
	for(int i = 0; i < datalen; i++){
		data[i] = 0;
	}
}

void ofApp::clearelet(int x){
	if(x == 0 || x == 2){
		for(int i = 0; i < elecrn; i++){
			for(int j = 0; j < eleccn; j++){
				elec[i][j] = 0;
			}
		}
	}
	if(x == 1 || x == 2){
		for(int i = 0; i < elecrn; i++){
			for(int j = 0; j < eleccn; j++){
				touch[i][j] = 0;
			}
		}
	}
	if(x == 3){
		for(int i = 0; i < elecrn; i++){
			for(int j = 0; j < eleccn; j++){
				elecgs[i][j] = 0;
			}
		}
	}
	if(x == 4){
		for(int i = 0; i < elecrn; i++){
			for(int j = 0; j < eleccn; j++){
				elecgc[i][j] = 0;
			}
		}
	}
	if(x == 5){
		for(int i = 0; i < dir; i++){
			vote[i] = 0;
		}
	}
}

void ofApp::captoelec(int cap, int no, int sf){
	int x = (cap * 10) + no;
	if(x == 11){
		elec[4][2] = 1;
	}
	else if(x == 12){
		elec[4][1] = 1;
	}
	else if(x == 13){
		elec[4][0] = 1;
	}
	else if(x == 14){
		elec[3][2] = 1;
	}
	else if(x == 15){
		elec[3][1] = 1;
	}
	else if(x == 16){
		elec[3][0] = 1;
	}
	else if(x == 17){
		elec[2][2] = 1;
	}
	else if(x == 18){
		elec[2][1] = 1;
	}
	else if(x == 21){
		elec[2][0] = 1;
	}
	else if(x == 22){
		elec[1][2] = 1;
	}
	else if(x == 23){
		elec[1][1] = 1;
	}
	else if(x == 24){
		elec[1][0] = 1;
	}
	else if(x == 25){
		elec[0][2] = 1;
	}
	else if(x == 26){
		elec[0][1] = 1;
	}
	else if(x == 27){
		elec[0][0] = 1;
	}
	else if(x == 28){
		elec[11][2] = 1;
	}
	else if(x == 31){
		elec[11][0] = 1;
	}
	else if(x == 32){
		elec[11][1] = 1;
	}
	else if(x == 33){
		elec[10][2] = 1;
	}
	else if(x == 34){
		elec[10][1] = 1;
	}
	else if(x == 35){
		elec[10][0] = 1;
	}
	else if(x == 36){
		elec[9][2] = 1;
	}
	else if(x == 37){
		elec[9][1] = 1;
	}
	else if(x == 38){
		elec[9][0] = 1;
	}
	else if(x == 41){
		elec[8][2] = 1;
	}
	else if(x == 42){
		elec[8][1] = 1;
	}
	else if(x == 43){
		elec[8][0] = 1;
	}
	else if(x == 44){
		elec[7][2] = 1;
	}
	else if(x == 45){
		elec[7][1] = 1;
	}
	else if(x == 46){
		elec[7][0] = 1;
	}
	else if(x == 47){
		elec[6][2] = 1;
	}
	else if(x == 48){
		elec[6][1] = 1;
	}
	else if(x == 51){
		elec[6][0] = 1;
	}
	if(x == 52){
		elec[5][2] = 1;
	}
	else if(x == 53){
		elec[5][1] = 1;
	}
	else if(x == 54){
		elec[5][0] = 1;
	}
}


void ofApp::captoelecr1(int cap, int no, int sf){
	int x = (cap * 10) + no;
	if(x == 11){
		elec[4][0] = 1;
	}
	else if(x == 12){
		elec[4][1] = 1;
	}
	else if(x == 13){
		elec[4][2] = 1;
	}
	else if(x == 14){
		elec[5][0] = 1;
	}
	else if(x == 15){
		elec[5][1] = 1;
	}
	else if(x == 16){
		elec[5][2] = 1;
	}
	else if(x == 17){
		elec[6][0] = 1;
	}
	else if(x == 18){
		elec[7][0] = 1;
	}
	else if(x == 24){
		elec[8][0] = 1;
	}
	else if(x == 25){
		elec[0][2] = 1;
	}
	else if(x == 26){
		elec[0][1] = 1;
	}
	else if(x == 27){
		elec[0][0] = 1;
	}
	else if(x == 28){
		elec[1][2] = 1;
	}
	else if(x == 31){
		elec[1][1] = 1;
	}
	else if(x == 32){
		elec[1][0] = 1;
	}
	else if(x == 33){
		elec[2][2] = 1;
	}
	else if(x == 34){
		elec[2][1] = 1;
	}
	else if(x == 35){
		elec[2][0] = 1;
	}
	else if(x == 36){
		elec[3][2] = 1;
	}
	else if(x == 37){
		elec[3][1] = 1;
	}
	else if(x == 38){
		elec[3][0] = 1;
	}
}


void ofApp::touchrecog(void){
	touchlog.erase(touchlog.begin());
	elecnlog.erase(elecnlog.begin());
	gesavg.erase(gesavg.begin());
	vector<vector<int>> v1;
	vector<int> v2;
	vector<vector<float>> v3;
	for(int i = 0; i < 3; i++){
		v1.push_back(logun);
		v2.push_back(-1);
		v3.push_back(logunf);
	}
	touchlog.push_back(v1);
	elecnlog.push_back(v2);
	gesavg.push_back(v3);

	if(capnum >= 0 || capflag == 1){
		//ex: (11, 2)
		//0~2: T, 3~5: P, 6~8: M, 9~11:B
		
		/*if(gcount >= ininoise){
			for(int i = 0; i <= 11; i++){	
				for(int j = 0; j < eleccn; j++){
					if(elec[i][j] == 1 && elecgs[i][j] == 1){
						elec[i][j] = 0;
					}
				}
			}
		}*/
		
		
		
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		trow = 0, tcol = 0;
		float maxwn = 0; 
		int maxar = -1, maxac = -1;
		int toulk = 0;
		int cc = 0, gcc = 0;
		thumbavg[0] = 0;
		thumbavg[1] = 0;
		gesavg[vl - 1][0][0] = 0;
		gesavg[vl - 1][0][1] = 0;
		for(int i = 0; i <= 5; i++){	//thumb+palm
			for(int j = 0; j < eleccn; j++){
				float rwn = 0; 
				int nbn = 0;
				thumbavg[0] += i;
				thumbavg[1] += j;
				checknb(i, j, 0, rwn, nbn);
				if(rwn > maxwn && nbn >= thnbn){
					maxwn = rwn;
					maxar = trow;
					maxac = tcol;
					toulk = 1;
				}
				if(elec[i][j] == 1){
					



					if(gcount < ininoise){
						elecgs[i][j] = 1;
					}
					else{
						if(elecgs[i][j] == 0){
							gesavg[vl - 1][0][0] += i;
							gesavg[vl - 1][0][1] += j;
							gcc++;
						}
					}


					cc++;
				}
			}
		}
		//cout<<gcount<<endl;
		if(gcount >= ininoise){
			if(iniflag == 0){
				cout<<"Initial\n";
				iniflag = 1;
			}
		}


		if(toulk == 1){
			//T+P: thumb touch
			if(0 <= maxar && maxar <= 5){
				touchlog[vl-1][0][0] = maxar;
				touchlog[vl-1][0][1] = maxac;
				touch[maxar][maxac] = 1;
			}
			thumbavg[0] = thumbavg[0] / cc;
			thumbavg[1] = thumbavg[1] / cc;
		}
		
		/*if(touchlog[vl-1][0] != logun && cseccfx[0] < touthf){
			if(touchlog[(vl-1) - 1][0] != logun){
				cseccfx[0]++;
			}
			else{
				cseccfx[0] = 1;
			}
		}
		else if(touchlog[vl-1][0] != logun && cseccfx[0] >= touthf){
			if(cseccfx[0] == touthf){
				gesstfx = touchlog[vl-1][0];
			}
			cseccfx[0]++;
		}

		if(cseccfx[0] > touthf){
			if(touchlog[vl-1][0] != logun){
				float curdistfx = dist(gesstfx, touchlog[vl-1][0]);
				if(curdistfx > maxdistffx){
					maxdistffx = curdistfx;
					gesmaxfx = touchlog[vl-1][0];
				}
			}
		}*/

		



		elecnlog[vl - 1][0] = gcc;
		if(gcc > 0 && cseccf[0] < touthf){
			gesavg[vl - 1][0][0] = gesavg[vl - 1][0][0] / gcc;
			gesavg[vl - 1][0][1] = gesavg[vl - 1][0][1] / gcc;
			if(elecnlog[(vl - 1) - 1][0] > 0){
				cseccf[0]++;
				//cout<<cseccf[0]<<endl;
			}
			else{
				cseccf[0] = 1;
			}
		}
		else if(gcc > 0 && cseccf[0] >= touthf){
			gesavg[vl - 1][0][0] = gesavg[vl - 1][0][0] / gcc;
			gesavg[vl - 1][0][1] = gesavg[vl - 1][0][1] / gcc;
			if(cseccf[0] == touthf){
				gesstf = gesavg[(vl - 1)][0];
				cout<<"gesstf r: "<<gesstf[0]<<"\tgesstf c: "<<gesstf[1]<<endl;
			}
			else if(cseccf[0] >= touthf){
				if(gesavg[vl - 1][0] != logunf && gesavg[(vl - 1) - 1][0] != logunf){
					if(gesavg[vl - 1][0][0] - gesavg[(vl - 1) - 1][0][0] < 0){
						vote[0]++;
					}
					else if(gesavg[vl - 1][0][0] - gesavg[(vl - 1) - 1][0][0] > 0){
						vote[1]++;
					}
					else if(gesavg[vl - 1][0][1] - gesavg[(vl - 1) - 1][0][1] < 0){
						vote[2]++;
					}
					else if(gesavg[vl - 1][0][1] - gesavg[(vl - 1) - 1][0][1] > 0){
						vote[3]++;
					}
				}
			}
			cseccf[0]++;
		}
		else if(gcc == 0){
			gesavg[vl - 1][0] = logunf;
		}

		if(cseccf[0] > touthf){
			if(gcc > 0){
				float curdistf = distf(gesstf, gesavg[vl - 1][0]);
				//cout<<curdistf<<endl;
				if(curdistf > maxdistff){
					maxdistff = curdistf;
					gesmaxf = gesavg[vl - 1][0];
				}
			}
		}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		gesavg[vl - 1][1][0] = 0;
		gesavg[vl - 1][1][1] = 0;
		trow = 0, tcol = 0, cc = 0, gcc = 0;
		maxwn = 0, maxar = -1, maxac = -1;
		toulk = 0;
		for(int i = 6; i <= 8; i++){	//middle finger
			for(int j = 0; j < eleccn; j++){
				float rwn = 0; 
				int nbn = 0;
				checknb(i, j, 0, rwn, nbn);
				if(rwn > maxwn && nbn >= thnbn){
					maxwn = rwn;
					maxar = trow;
					maxac = tcol;
					toulk = 1;
				}
				if(elec[i][j] == 1){
					



					if(gcount < ininoise){
						elecgs[i][j] = 1;
					}
					else{
						if(elecgs[i][j] == 0){
							gesavg[vl - 1][1][0] += i;
							gesavg[vl - 1][1][1] += j;
							gcc++;
						}
					}



					cc++;
				}
			}
		}



		if(toulk == 1){
			//M: middle finger touch
			if(6 <= maxar && maxar <= 8){
				touchlog[vl-1][1][0] = maxar;
				touchlog[vl-1][1][1] = maxac;
				touch[maxar][maxac] = 1;
			}
		}

		/*if(touchlog[vl-1][1] != logun && cseccfx[1] < touthf){
			if(touchlog[(vl-1) - 1][1] != logun){
				cseccfx[1]++;
			}
			else{
				cseccfx[1] = 1;
			}
		}
		else if(touchlog[vl-1][1] != logun && cseccfx[1] >= touthf){
			if(cseccfx[1] == touthf){
				gesstfx = touchlog[vl-1][1];
			}
			cseccfx[1]++;
		}*/





		elecnlog[vl - 1][1] = gcc;
		if(gcc > mideln && cseccf[1] < touthf){
			gesavg[vl - 1][1][0] = gesavg[vl - 1][1][0] / gcc;
			gesavg[vl - 1][1][1] = gesavg[vl - 1][1][1] / gcc;
			if(elecnlog[(vl - 1) - 1][1] > 0){
				cseccf[1]++;
			}
			else{
				cseccf[1] = 1;
			}
		}
		else if(gcc > mideln && cseccf[1] >= touthf){
			gesavg[vl - 1][1][0] = gesavg[vl - 1][1][0] / gcc;
			gesavg[vl - 1][1][1] = gesavg[vl - 1][1][1] / gcc;
			if(cseccf[1] == touthf){
				//gesstf = gesavg[(vl - 1)][1];
			}
			cseccf[1]++;
		}
		else if(gcc <= mideln){
			gesavg[vl - 1][1] = logunf;
		}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		gesavg[vl - 1][2][0] = 0;
		gesavg[vl - 1][2][1] = 0;
		trow = 0, tcol = 0, cc = 0;
		maxwn = 0, maxar = -1, maxac = -1;
		toulk = 0;
		for(int i = 9; i <= 11; i++){	//back
			for(int j = 0; j < eleccn; j++){
				float rwn = 0; 
				int nbn = 0;
				checknb(i, j, 0, rwn, nbn);
				if(rwn > maxwn && nbn >= thnbn){
					maxwn = rwn;
					maxar = trow;
					maxac = tcol;
					toulk = 1;
				}
				if(elec[i][j] == 1){
					



					if(gcount < ininoise){
						elecgs[i][j] = 1;
					}
					else{
						if(elecgs[i][j] == 0){
							gesavg[vl - 1][2][0] += i;
							gesavg[vl - 1][2][1] += j;
							gcc++;
						}
					}



					cc++;
				}
			}
		}



		if(toulk == 1){
			//B: two-handed touch
			if(9 <= maxar && maxar <= 11){
				touchlog[vl-1][2][0] = maxar;
				touchlog[vl-1][2][1] = maxac;
				touch[maxar][maxac] = 1;
				cout<<"Two-handed touch"<<endl;
			}
		}

		/*if(touchlog[vl-1][2] != logun && cseccfx[2] < touthf){
			if(touchlog[(vl-1) - 1][2] != logun){
				cseccfx[2]++;
			}
			else{
				cseccfx[2] = 1;
			}
		}
		else if(touchlog[vl-1][2] != logun && cseccfx[2] >= touthf){
			if(cseccfx[2] == touthf){
				gesstfx = touchlog[vl-1][2];
			}
			cseccfx[2]++;
		}*/





		elecnlog[vl - 1][2] = gcc;
		if(gcc > 0 && cseccf[2] < touthf){
			gesavg[vl - 1][2][0] = gesavg[vl - 1][2][0] / gcc;
			gesavg[vl - 1][2][1] = gesavg[vl - 1][2][1] / gcc;
			if(elecnlog[(vl - 1) - 1][2] > 0){
				cseccf[2]++;
			}
			else{
				cseccf[2] = 1;
			}
		}
		else if(gcc > 0 && cseccf[2] >= touthf){
			gesavg[vl - 1][2][0] = gesavg[vl - 1][2][0] / gcc;
			gesavg[vl - 1][2][1] = gesavg[vl - 1][2][1] / gcc;
			if(cseccf[2] == touthf){
				//gesstf = gesavg[(vl - 1)][2];
			}
			cseccf[2]++;
		}
		else if(gcc == 0){
			gesavg[vl - 1][2] = logunf;
		}

	}
	/*for(int i = 0; i < vl; i++){
		cout<<endl;
		for(int j = 0; j < 3; j++){
			cout<<touchlog[i][j][0]<<" ";
			cout<<touchlog[i][j][1]<<"\t";
		}
	}
	cout<<endl;*/
	else{
		//cout<<"capflag: "<<capflag<<endl;
		//cout<<"capnum: "<<capnum<<endl;
	}
}

void ofApp::checknb(int row, int col, int flag, float & rwn, int & nbn){
	float num = 0, wn = 0;
	float srow = 0, scol = 0;
	int	mwl = 2, mwd = 1;	//mask weighting line / diagonal
	if(flag == 0){			//electrodes
		if(col == 0){
			if(row == 0){
				if(elec[elecrn-1][col] == 1){
					num++;
					wn = wn + mwl;
					srow = srow + ((row - 1)*mwl);
					scol = scol + (col*mwl);
				}
				if(elec[elecrn-1][col + 1] == 1){
					num++;
					wn = wn + mwd;
					srow = srow + ((row - 1)*mwd);
					scol = scol + ((col + 1)*mwd);
				}
			}
			else{
				if(elec[row - 1][col] == 1){
					num++;
					wn = wn + mwl;
					srow = srow + ((row - 1)*mwl);
					scol = scol + (col*mwl);
				}
				if(elec[row - 1][col + 1] == 1){
					num++;
					wn = wn + mwd;
					srow = srow + ((row - 1)*mwd);
					scol = scol + ((col + 1)*mwd);
				}
			}

			if(elec[row][col] == 1){
				num++;
				wn = wn + mwl;
				srow = srow + (row*mwl);
				scol = scol + (col*mwl);
			}
			if(elec[row][col + 1] == 1){
				num++;
				wn = wn + mwl;
				srow = srow + (row*mwl);
				scol = scol + ((col + 1)*mwl);
			}

			if(row == (elecrn-1)){
				if(elec[0][col] == 1){
					num++;
					wn = wn + mwl;
					srow = srow + ((row + 1)*mwl);
					scol = scol + (col*mwl);
				}
				if(elec[0][col + 1] == 1){
					num++;
					wn = wn + mwd;
					srow = srow + ((row + 1)*mwd);
					scol = scol + ((col + 1)*mwd);
				}
			}
			else{
				if(elec[row + 1][col] == 1){
					num++;
					wn = wn + mwl;
					srow = srow + ((row + 1)*mwl);
					scol = scol + (col*mwl);
				}
				if(elec[row + 1][col + 1] == 1){
					num++;
					wn = wn + mwd;
					srow = srow + ((row + 1)*mwd);
					scol = scol + ((col + 1)*mwd);
				}
			}
		}
		else if(col == 1){
			if(row == 0){
				if(elec[elecrn-1][col - 1] == 1){
					num++;
					wn = wn + mwd;
					srow = srow + ((row - 1)*mwd);
					scol = scol + ((col - 1)*mwd);
				}
				if(elec[elecrn-1][col] == 1){
					num++;
					wn = wn + mwl;
					srow = srow + ((row - 1)*mwl);
					scol = scol + (col*mwl);
				}
				if(elec[elecrn-1][col + 1] == 1){
					num++;
					wn = wn + mwd;
					srow = srow + ((row - 1)*mwd);
					scol = scol + ((col + 1)*mwd);
				}
			}
			else{
				if(elec[row - 1][col - 1] == 1){
					num++;
					wn = wn + mwd;
					srow = srow + ((row - 1)*mwd);
					scol = scol + ((col - 1)*mwd);
				}
				if(elec[row - 1][col] == 1){
					num++;
					wn = wn + mwl;
					srow = srow + ((row - 1)*mwl);
					scol = scol + (col*mwl);
				}
				if(elec[row - 1][col + 1] == 1){
					num++;
					wn = wn + mwd;
					srow = srow + ((row - 1)*mwd);
					scol = scol + ((col + 1)*mwd);
				}
			}

			if(elec[row][col - 1] == 1){
				num++;
				wn = wn + mwl;
				srow = srow + (row*mwl);
				scol = scol + ((col - 1)*mwl);
			}
			if(elec[row][col] == 1){
				num++;
				wn = wn + mwl;
				srow = srow + (row*mwl);
				scol = scol + (col*mwl);
			}
			if(elec[row][col + 1] == 1){
				num++;
				wn = wn + mwl;
				srow = srow + (row*mwl);
				scol = scol + ((col + 1)*mwl);
			}

			if(row == (elecrn-1)){
				if(elec[0][col - 1] == 1){
					num++;
					wn = wn + mwd;
					srow = srow + ((row + 1)*mwd);
					scol = scol + ((col - 1)*mwd);
				}
				if(elec[0][col] == 1){
					num++;
					wn = wn + mwl;
					srow = srow + ((row + 1)*mwl);
					scol = scol + (col*mwl);
				}
				if(elec[0][col + 1] == 1){
					num++;
					wn = wn + mwd;
					srow = srow + ((row + 1)*mwd);
					scol = scol + ((col + 1)*mwd);
				}
			}
			else{
				if(elec[row + 1][col - 1] == 1){
					num++;
					wn = wn + mwd;
					srow = srow + ((row + 1)*mwd);
					scol = scol + ((col - 1)*mwd);
				}
				if(elec[row + 1][col] == 1){
					num++;
					wn = wn + mwl;
					srow = srow + ((row + 1)*mwl);
					scol = scol + (col*mwl);
				}
				if(elec[row + 1][col + 1] == 1){
					num++;
					wn = wn + mwd;
					srow = srow + ((row + 1)*mwd);
					scol = scol + ((col + 1)*mwd);
				}
			}
		}
		else{
			if(row == 0){
				if(elec[elecrn-1][col - 1] == 1){
					num++;
					wn = wn + mwd;
					srow = srow + ((row - 1)*mwd);
					scol = scol + ((col - 1)*mwd);
				}
				if(elec[elecrn-1][col] == 1){
					num++;
					wn = wn + mwl;
					srow = srow + ((row - 1)*mwl);
					scol = scol + (col*mwl);
				}
			}
			else{
				if(elec[row - 1][col - 1] == 1){
					num++;
					wn = wn + mwd;
					srow = srow + ((row - 1)*mwd);
					scol = scol + ((col - 1)*mwd);
				}
				if(elec[row - 1][col] == 1){
					num++;
					wn = wn + mwl;
					srow = srow + ((row - 1)*mwl);
					scol = scol + (col*mwl);
				}
			}

			if(elec[row][col - 1] == 1){
				num++;
				wn = wn + mwl;
				srow = srow + (row*mwl);
				scol = scol + ((col - 1)*mwl);
			}
			if(elec[row][col] == 1){
				num++;
				wn = wn + mwl;
				srow = srow + (row*mwl);
				scol = scol + (col*mwl);
			}

			if(row == (elecrn-1)){
				if(elec[0][col - 1] == 1){
					num++;
					wn = wn + mwd;
					srow = srow + ((row + 1)*mwd);
					scol = scol + ((col - 1)*mwd);
				}
				if(elec[0][col] == 1){
					num++;
					wn = wn + mwl;
					srow = srow + ((row + 1)*mwl);
					scol = scol + (col*mwl);
				}
			}
			else{
				if(elec[row + 1][col - 1] == 1){
					num++;
					wn = wn + mwd;
					srow = srow + ((row + 1)*mwd);
					scol = scol + ((col - 1)*mwd);
				}
				if(elec[row + 1][col] == 1){
					num++;
					wn = wn + mwl;
					srow = srow + ((row + 1)*mwl);
					scol = scol + (col*mwl);
				}
			}
		}
		if(num > 0){
			if(srow >= 0){
				trow = ((srow / wn) + 0.5);
			}
			else{
				trow = ((srow / wn) - 0.5);
			}
			tcol = ((scol / wn) + 0.5);
			
			if(trow >= elecrn){
				trow = trow - elecrn;
			}
			else if(trow < 0){
				trow = trow + elecrn;
			}
		}
		rwn = wn/num;
		nbn = num;
	}
	else if(flag == 1){			//touch
	}
}

void ofApp::delayt(int ms){
	clock_t start = clock(), stop = clock();
	while(stop - start < ms){
		stop = clock();
	}
}

void ofApp::testele(void){			//T: 0~2, P: 3~5, M: 6~8, B: 9~11
	elec[0][0] = 0;	 elec[0][1] = 0;	 elec[0][2] = 0;
	elec[1][0] = 1;	 elec[1][1] = 1;	 elec[1][2] = 0;
	elec[2][0] = 1;	 elec[2][1] = 0;	 elec[2][2] = 0;
	elec[3][0] = 0;	 elec[3][1] = 0;	 elec[3][2] = 0;
	elec[4][0] = 0;	 elec[4][1] = 0;	 elec[4][2] = 0;
	elec[5][0] = 0;	 elec[5][1] = 0;	 elec[5][2] = 0;
	
	elec[6][0] = 0;	 elec[6][1] = 0;	 elec[6][2] = 0;
	elec[7][0] = 0;	 elec[7][1] = 0;	 elec[7][2] = 0; 
	elec[8][0] = 0;	 elec[8][1] = 0;	 elec[8][2] = 0;
	 
	elec[9][0] = 0;	 elec[9][1] = 1;	 elec[9][2] = 1;
	elec[10][0] = 1;	 elec[10][1] = 1;	 elec[10][2] = 1;
	elec[11][0] = 0;	 elec[11][1] = 1;	 elec[11][2] = 0;
	capnum = 1;
}

void ofApp::gesrecog(void){
	gesture = 0;
	if(mode == 0){			//none
	}
	else if(mode == 1){	//recognition experiment
		if(move == -1){
			int csecth = 1;
			int taputh = 200, taplth = 200;
			for(int i = 0; i < mtn; i++){				//check move
				if(touchlog[vl-1][i] == csecv[i][0]/* && csecc[i][1] != 0*/){
					//csecc[i][0]++;						//orign
					//if(csecc[i][0] > csecth){			//do not move
					//	csecc[i][0] = 0;
						csecc[i][1] = 0;
						csecv[i][1] = logun;
					//}
				}
				else if(touchlog[vl-1][i] == csecv[i][1]){
					csecc[i][1]++;						//change
					if(csecc[i][1] >= csecth){			//move successfully

						////////////////////////////////////////////////////////////////////////////////////////
						if(csecv[i][0] == logun && csecv[i][1] != logun){	//touch
							touched[i] = 1;
							touchst[i] = clock();
							if(i == 0){
								cout<<"touched"<<endl;
							}
							else if(i == 1){
								//cout<<"middle finger touched"<<endl;
							}
						}
						else if(csecv[i][0] != logun && csecv[i][1] != logun){	//move
							if(i == 0){
								move = i;					//the finger moves
								gesst = csecv[i][0];
								gesmaxd = csecv[i][1];
								maxdist = dist(gesst, gesmaxd);
								cout<<"move"<<endl;
							}
							//else if(i == 1 && touched[0] == 1){			//middle finger move only when thumb touches
							//	move = i;
							//	gesst = csecv[i][0];
							//}
						}
						else if(csecv[i][0] != logun && csecv[i][1] == logun){	//lift, touch and lift but don't move
							if(i == 0){								//thumb might be tap
								clock_t currt = clock();
								if(/*(currt - touchst[i] < taputh) && */(currt - touchst[i] > taplth)){		//tap
									if(touched[1] == 1){			//middle finger touched
										gesture = 115;
									}
									else if(touched[1] == 0){		//middle finger untouched
										gesture = 215;
									}
								}
								cout<<"lift"<<endl;
							}
							touched[i] = 0;
						}
						//////////////////////////////////////////////////////////////////////////////////////////

						if(move != -1){
							csecv[i][0] = logun;	
						}
						else{
							csecv[i][0] = csecv[i][1];
							ms = thumbavg;
						}
						//csecc[i][0] = 0;
						csecc[i][1] = 0;
						csecv[i][1] = logun;
					}
				}
				else{
					csecc[i][1] = 1;
					csecv[i][1] = touchlog[vl-1][i];
				}
			}
		}
		else{		//moved
			int csecth = 2;
			int swipelth = 200;
			clock_t currt = clock();
			if(touchlog[vl-1][move] == logun && csecc[move][1] < csecth){
				if(nonlift == 1){
					csecc[move][1] = 1;
				}
				else{
					csecc[move][1]++;
				}
				nonlift = 0;
			}
			else if(touchlog[vl-1][move] == logun && csecc[move][1] >= csecth){			//lift
				if(currt - touchst[move] > swipelth){
					gesend = touchlog[(vl-1) - (csecth + 1)][move];
					/*int r = gesend[0] - gesst[0];		//start to end
					int c = gesend[1] - gesst[1];*/
					int r = gesmaxd[0] - gesst[0];		//start to max distance
					int c = gesmaxd[1] - gesst[1];
					float rf = mmax[0] - ms[0];			//start to mean max distance
					float cf = mmax[1] - ms[1];
					cout<<"\nmmax r: "<<mmax[0]<<"\tms r: "<<ms[0]<<"\tmmax c: "<<mmax[1]<<"\tms c: "<<ms[1]<<endl;
					cout<<"\ngesmaxd r: "<<gesmaxd[0]<<"\tgesst r: "<<gesst[0]<<"\tgesmaxd c: "<<gesmaxd[1]<<"\tgesst c: "<<gesst[1]<<endl;
					cout<<"\ngesend r: "<<gesend[0]<<"\tgesst r: "<<gesst[0]<<"\tgesend c: "<<gesend[1]<<"\tgesst c: "<<gesst[1]<<endl;
					if(move == 0){
						if(abs(rf) > abs(cf) && rf < 0){
							//up
							if(touched[1] == 1){
								gesture = 111;
							}
							else{
								gesture = 211;
							}
						}
						else if(abs(rf) > abs(cf) && rf >= 0){
							//down
							if(touched[1] == 1){
								gesture = 112;
							}
							else{
								gesture = 212;
							}
						}
						else if(abs(rf) <= abs(cf) && cf >= 0){
							//right
							if(touched[1] == 1){
								gesture = 114;
							}
							else{
								gesture = 214;
							}
						}
						else if(abs(rf) <= abs(cf) && cf < 0){
							//left
							if(touched[1] == 1){
								gesture = 113;
							}
							else{
								gesture = 213;
							}
						}
					}
					else if(move == 1){
						if(r >= 0){
							//up
							gesture = 321;
						}
						else if(r < 0){
							//down
							gesture = 322;
						}
					}
				}
				maxdist = 0, maxdistf = 0;
				move = -1;
				csecc[move][1] = 0;
				nonlift = 0;
			}
			else{
				float curdist = dist(gesst, touchlog[vl-1][move]);
				if(curdist > maxdist){
					maxdist = curdist;
					gesmaxd = touchlog[vl-1][move];
				}

				float curdistf = distf(ms, thumbavg);
				if(curdistf > maxdistf){
					maxdistf = curdistf;
					mmax = thumbavg;
				}

				nonlift = 1;
			}
		}

	}
}

void ofApp::gesrecogg(void){
	gesture = 0;
	//gesture1 = 0;
	//int tapthf = 1;
	if(gesstf != logunf){
		if(gesmaxf != logunf){
			float rf = gesmaxf[0] - gesstf[0];
			float cf = gesmaxf[1] - gesstf[1];
			float vr = vote[0] - vote[1];  //vr > 0 up
			float vc = vote[2] - vote[3];  //vc > 0 left
			cout<<"\ngesmaxf r: "<<gesmaxf[0]<<"\tgesstf r: "<<gesstf[0]<<"\tgesmaxf c: "<<gesmaxf[1]<<"\tgesstf c: "<<gesstf[1]<<"\tmiddle: "<<cseccf[1]<<endl;	
			cout<<"rf: "<<rf<<"\tcf: "<<cf/*<<"\tvr: "<<vr<<"\tvc: "<<vc*/<<endl;
			if(trainflag == 1){
				if(abs(cf) > abs(tcf)){
					tcf = cf;
				}
				if(abs(rf) > abs(trf)){
					trf = rf;
				}

				if(tswipe == 1 || tswipe ==3){	//up or left
					if(gesmaxf[0] < tmaxr){
						tmaxr = gesmaxf[0];
					}
					if(gesstf[0] > tstr){
						tstr = gesstf[0];
					}
					if(gesmaxf[1] < tmaxc){
						tmaxc = gesmaxf[1];
					}
					if(gesstf[1] > tstc){
						tstc = gesstf[1];
					}
				}
				else if(tswipe == 2 || tswipe == 4){	//down or right
					if(gesmaxf[0] > tmaxr){
						tmaxr = gesmaxf[0];
					}
					if(gesstf[0] < tstr){
						tstr = gesstf[0];
					}
					if(gesmaxf[1] > tmaxc){
						tmaxc = gesmaxf[1];
					}
					if(gesstf[1] < tstc){
						tstc = gesstf[1];
					}
				}
				if(cseccf[1] > tmtn){
					tmtn = cseccf[1];
				}
				cout<<"tcf: "<<tcf<<"\ttrf: "<<trf<</*"\ttmaxr: "<<tmaxr<<"\ttstr: "<<tstr<<"\ttmaxc: "<<tmaxc<<"\ttstc: "<<tstc<<*/"\ttmtn: "<<tmtn<<endl;
				tcount++;
				cout<<"Training count: "<<tcount<<endl;
			}
			

			if(abs(rf) <= tapth && abs(cf) <= tapth){
				//tap
				if(cseccf[1] > mdftftap){
					gesture = 115;
				}
				else{
					gesture = 215;
				}
			}
			else if(rf > 0 && abs(rf) >= downrth){
				//down
				if(cseccf[1] > mdftf){
					gesture = 112;
				}
				else{
					gesture = 212;
				}
			}
			else if(rf <= 0 && abs(rf) >= uprth){
				//up
				if(cseccf[1] > mdftf){
					gesture = 111;
				}
				else{
					gesture = 211;
				}
			}
			else{
				//if(abs(rf) > abs(cf) && rf <= 0){
				//	//up
				//	if(cseccf[1] > mdftf){
				//		gesture = 111;
				//	}
				//	else{
				//		gesture = 211;
				//	}
				//}
				//else if(abs(rf) > abs(cf) && rf > 0){
				//	//down
				//	if(cseccf[1] > mdftf){
				//		gesture = 112;
				//	}
				//	else{
				//		gesture = 212;
				//	}
				//}
				/*else*/ if(/*abs(rf) <= abs(cf) &&*/ cf > 0){
					//right
					if(cseccf[1] > mdftf){
						gesture = 114;
					}
					else{
						gesture = 214;
					}
				}
				else if(/*abs(rf) <= abs(cf) &&*/ cf <= 0){
					//left
					if(cseccf[1] > mdftf){
						gesture = 113;
					}
					else{
						gesture = 213;
					}
				}
			}
		}
		else{
			//tap
			if(cseccf[1] > mdftftap){
				gesture = 115;
			}
			else{
				gesture = 215;
			}
		}
	}
	else{
		//tap
		if(cseccf[1] > mdftftap){
			gesture = 115;
		}
		else{
			gesture = 215;
		}
	}


	//if(gesstfx != logun){
	//	int tapthf = 10, mdftf = 10;
	//	float rf = gesmaxfx[0] - gesstfx[0];
	//	float cf = gesmaxfx[1] - gesstfx[1];
	//	cout<<"\ngesmaxfx r: "<<gesmaxfx[0]<<"\tgesstfx r: "<<gesstfx[0]<<"\tgesmaxfx c: "<<gesmaxfx[1]<<"\tgesstfx c: "<<gesstfx[1]<<endl;	
	//	if(abs(rf) > abs(cf) && rf < 0){
	//		//up
	//		if(cseccfx[1] > mdftf){
	//			gesture1 = 111;
	//		}
	//		else{
	//			gesture1 = 211;
	//		}
	//	}
	//	else if(abs(rf) > abs(cf) && rf > 0){
	//		//down
	//		if(cseccfx[1] > mdftf){
	//			gesture1 = 112;
	//		}
	//		else{
	//			gesture1 = 212;
	//		}
	//	}
	//	else if(abs(rf) <= abs(cf) && cf > 0){
	//		//right
	//		if(cseccfx[1] > mdftf){
	//			gesture1 = 114;
	//		}
	//		else{
	//			gesture1 = 214;
	//		}
	//	}
	//	else if(abs(rf) <= abs(cf) && cf < 0){
	//		//left
	//		if(cseccfx[1] > mdftf){
	//			gesture1 = 113;
	//		}
	//		else{
	//			gesture1 = 213;
	//		}
	//	}
	//	else if(rf == cf && rf == 0 && cseccfx[0] > tapthf){
	//		if(cseccfx[1] > mdftf){
	//			gesture1 = 115;
	//		}
	//		else{
	//			gesture1 = 215;
	//		}
	//	}
	//}
	//else{
	//	cout<<"none gesture1"<<endl;
	//}
	
	cseccf[0] = 0, cseccf[1] = 0, cseccf[2] = 0;
	cseccfx[0] = 0, cseccfx[1] = 0, cseccfx[2] = 0;
	maxdistff = 0, maxdistffx = 0;
	gesmaxf = logunf, gesmaxfx = logun;
	gesstf = logunf, gesstfx = logun;
	iniflag = 0;
	clearelet(3);
	clearelet(5);
}

void ofApp::initvec(void){
	vector<vector<int>> v1, v2;
	vector<int> v3;
	v2.push_back(logun);
	v2.push_back(logun);
	vector<vector<float>> v4;
	for(int i = 0; i < 3; i++){
		v1.push_back(logun);
		csecv.push_back(v2);
		v3.push_back(-1);	
		v4.push_back(logunf);
	}
	for(int i = 0; i < vl; i++){
		touchlog.push_back(v1);
		gesavg.push_back(v4);
		elecnlog.push_back(v3);
	}
	
}

float ofApp::dist(vector<int> a, vector <int> b){
	float dis = 0;
	dis = (pow((a[1] - b[1]), 2) + pow((a[0] - b[0]), 2));
	dis = pow(dis, 0.5);
	return dis;
}

float ofApp::distf(vector<float> a, vector <float> b){
	float dis = 0;
	dis = (pow((a[1] - b[1]), 2) + pow((a[0] - b[0]), 2));
	dis = pow(dis, 0.5);
	return dis;
}


void ofApp::build_gesture_type(void){
    
	gesturetype.push_back(make_pair("Touched, up", 111));
    gesturetype.push_back(make_pair("Touched, down", 112));
	gesturetype.push_back(make_pair("Touched, left", 113));
	gesturetype.push_back(make_pair("Touched, right", 114));
    gesturetype.push_back(make_pair("Touched, tap", 115));
    gesturetype.push_back(make_pair("Unouched, up", 211));
    gesturetype.push_back(make_pair("Unouched, down", 212));
    gesturetype.push_back(make_pair("Unouched, left", 213));
    gesturetype.push_back(make_pair("Unouched, right", 214));
    gesturetype.push_back(make_pair("Unouched,  tap", 215));
    //gesturetype.push_back(make_pair("Thumb touched,\nMiddle finger swipe up", 321));
    //gesturetype.push_back(make_pair("Thumb touched,\nMiddle finger swipe down", 322));

    /*for (int i = 0; i<gesturetype.size(); i++)
        cout << gesturetype[i].first <<endl;*/
    
}

void ofApp::exre_init(void){
    //Touch:	MT t: 1xx, MT u: 2xx, TT t: 3xx
    //TG x1x:	111: tTGup, 211: uTGup, 112: tTGdown, 212: uTGdown, 113: tTGleft, 213: uTGleft
    //			114: tTGright, 214: uTGright, 115: tTGtap, 215: uTGtap
    //MG x2x:	321: tMGup, 322: tMGdown
    
    fstream fp;
    fp.open("exrelog.txt", ios::out);
    fp.close();
	//fp.open("exrelog1.txt", ios::out);
    //fp.close();
    
	exre_array.clear();
    exre_array.push_back(make_pair("press space to start!", 0));
    for (int i = 0; i < gesturetype.size(); ++i)
        for (int j = 0; j < exretimes; ++j)
            exre_array.push_back(gesturetype[i]);
     
    exre_array.push_back(make_pair("exrecog over\n thank u!", 0));
	random_shuffle( exre_array.begin()+1, exre_array.end()-1);

    exrenow = 0;
	exrecolor = 0;
    exrecorrect = 0;
    exreflag = 0;
    exreges = 0;
	exrecorrect1 = 0;
    exreflag1 = 0;
    exreges1 = 0;
	writeflag = true;
     
    return;
}

void ofApp::exrecog(int ges){
    if (ges == 0)
        return;
     
    if (exreflag == 1){
        if (ges == exre_array[exrenow].second){
            exrecorrect++;
            exrehit = 1;
            cout << exre_array[exrenow].second << "\t" << exrehit << "\t" << ges <<endl;
            exrecolor = 1;
        }
        else {
            exrehit = 0;
            cout << exre_array[exrenow].second << "\t" << exrehit << "\t" << ges <<endl;
            exrecolor = 2;
        }
        exreflag = 0;
        exreges = ges;
    }
    return;
}

void ofApp::exrecog1(int ges){
    if (ges == 0)
        return;
     
    if (exreflag1 == 1){
        if (ges == exre_array[exrenow].second){
            exrecorrect1++;
            exrehit1 = 1;
            cout << exre_array[exrenow].second << "\t" << exrehit1 << "\t" << ges <<endl;
        }
        else {
            exrehit1 = 0;
            cout << exre_array[exrenow].second << "\t" << exrehit1 << "\t" << ges <<endl;
        }
        exreflag1 = 0;
        exreges = ges;
    }
    return;
}

string ofApp::exreinttostr(int ges){
    string res;
    switch(ges){
        case 111:
            res = "tTGup";
            break;
        case 211:
            res = "uTGup";
            break;
        case 112:
            res = "tTGdown";
            break;
        case 212:
            res = "uTGdown";
            break;
        case 113:
            res = "tTGleft";
            break;
        case 213:
            res = "uTGleft";
            break;
        case 114:
            res = "tTGright";
            break;
        case 214:
            res = "uTGright";
            break;
        case 115:
            res = "tTGtap";
            break;
        case 215:
            res = "uTGtap";
            break;
        case 321:
            res = "tMGup";
            break;
        case 322:
            res = "tMGdown";
            break;
        default:
            res = "none";
    }
    return res;
}

void ofApp::icon2d(int act){
    if (act == 1){
        if (cursorX == targetX && cursorY == targetY){
            iconcorrect++;
            iconhit = 1;
        }
        else
            iconhit = 0;
        icon2d_init();
    }

}

void ofApp::icon2d_init(void){
    iconheight = 6;
    iconwidth = 4;
    targetX = rand()%iconwidth;
    targetY = rand()%iconheight;
}

