#pragma once
#include "ofMain.h"


class nameStruct{
public: 
    ofPoint point;
    string theName;
    int count;
    
    nameStruct();
    nameStruct(ofPoint _point, string _theName, int _count){
        point = _point;
        theName = _theName;
        count = _count;
    }
    
    ofPoint getLocationFromName(string name){
        if(name == theName) return point;
        else cout<< "Name not Found";
        }
    string getName(){return theName;}
    int getCount(){return count;}
    
    void setLocation(float x, float y){
        point.x = x;
        point.y = y;
    }
    void setName(string _name){theName = _name;}
    void setCount(int _i){count = _i;}
    friend class testApp;
    friend struct foundPair;
};

struct foundPair{
    ofPoint textOne;
    ofPoint textTwo;
    string theMatchedName;
   
    foundPair(ofPoint _textOne, ofPoint _textTwo, string _theMatchedName){
        textOne = _textOne;
        textTwo = _textTwo;
        theMatchedName = _theMatchedName;
    }
};

class testApp : public ofBaseApp{
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
    
        vector <string> cleanPunctuation(string filename);
        int isCommonWord(string _testString, vector<string> _ignoreWords);
        vector<string> properNames(vector<string> _words);
        void writeNamesToFile(vector<string> _names, string toFilePath);
        vector<string> uniqueNames(vector<string> _words);
        nameStruct createNameStructs(string token, vector<string>_baseText, int flagPosition);
        vector<nameStruct> createVisualizationVector(string pathToData, int flagPosition);
        float minLocation(vector<nameStruct> _structofNames);
        float maxLocation(vector<nameStruct>_structofNames);
        static bool sortOnAppearances(const nameStruct &alpha, const nameStruct &beta);
        static bool sortOnName(const nameStruct &alpha, const nameStruct &beta);
        void drawVizVector(vector<nameStruct> & _theVectortoDraw, int _startItems, int _numItems, int _fontSize, 
                           bool _connectionMoused = FALSE);
        vector<foundPair > findConnection(vector<nameStruct> &textOne, vector<nameStruct> &textTwo, int _startItems, int _numItems, int _numItemsTwo);
        void drawConnection(vector<foundPair> &foundMatches, bool _connectionMoused = FALSE);
        void highlightConnection(foundPair &highlightedMatch);
        void drawName(nameStruct &text);
        void drawBackground();
    
        int numItems;
        int numItemsTwo;
        vector<nameStruct> vizQuran;
        vector<nameStruct> vizNT;
        vector<foundPair> QandNT;
        ofTrueTypeFont myFont;
        bool connectionMousedOver;
      
};



