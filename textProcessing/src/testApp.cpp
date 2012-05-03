#include "testApp.h"
#include "Poco/RegularExpression.h"
#include "Poco/UTF8String.h"
#include "Poco/String.h"
using Poco::RegularExpression;
using Poco::UTF8;
using Poco::icompare;

int startItems =0;

bool testApp::sortOnAppearances(const nameStruct &alpha, const nameStruct &beta){
    return (alpha.count > beta.count);
}

bool testApp::sortOnName(const nameStruct &alpha, const nameStruct &beta){
    return (alpha.theName < beta.theName);
}

//--------------------------------------------------------------
void testApp::setup(){
    
    ofBackground(0,0,0);
    connectionMousedOver = FALSE;
    vizQuran = createVisualizationVector("Quran.txt", 0);
    vizNT  = createVisualizationVector("newTestament.txt",1);
        
    numItems = vizQuran.size();
    numItemsTwo = vizNT.size();
    myFont.loadFont("Verdana.ttf", 20, true, true, true);
    sort(vizQuran.begin(), vizQuran.end(), testApp::sortOnAppearances);
    sort(vizNT.begin(), vizNT.end(), testApp::sortOnAppearances);
    QandNT = findConnection(vizQuran, vizNT, startItems, numItems, numItemsTwo);
    
    /*ofBuffer ntFile = ofBufferFromFile("newTestament.txt");
        string ntAsString = ntFile;
        RegularExpression myRegEx("[0-9]+:[0-9]+"); 
        RegularExpression::Match match;
        int found = myRegEx.subst(ntAsString," ", 0x10000000);
        myRegEx.subst(ntAsString, " ");
        ntFile.set(ntAsString.c_str(), ntAsString.size());
        ofBufferToFile("newTestament2.txt", ntFile);*/
    
        
}

//--------------------------------------------------------------
void testApp::update(){
QandNT = findConnection(vizQuran, vizNT, startItems, numItems, numItemsTwo);

}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255,255,255,50);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    int fontSize = 12;
    drawVizVector(vizQuran, startItems, numItems, fontSize, connectionMousedOver); 
    drawVizVector(vizNT, startItems, numItemsTwo, fontSize, connectionMousedOver);
    ofSetColor(50,255,50,75);
    drawConnection(QandNT, connectionMousedOver);
        if(!QandNT.empty()){
            myFont.loadFont("Verdana.ttf", 20, true, true, true);
            for(vector<foundPair>::iterator it = QandNT.begin(); it < QandNT.end(); it++){
            if(((mouseX >= (int) it->textOne.x && mouseX <= (int) (it->textOne.x + myFont.stringWidth(it->theMatchedName))) && 
                (mouseY <= (int) it->textOne.y && mouseY >= (int)(it->textOne.y - myFont.stringHeight(it->theMatchedName)))) ||
               ((mouseX >= (int) it->textTwo.x && mouseX <= (int) (it->textTwo.x + myFont.stringWidth(it->theMatchedName))) && 
                (mouseY <= (int) it->textTwo.y && mouseY >= (int)(it->textTwo.y - myFont.stringHeight(it->theMatchedName))))){
                   connectionMousedOver = TRUE;
                   //cout << "connection";
                   nameStruct left = nameStruct(it->textOne, it->theMatchedName, 0);
                   nameStruct right = nameStruct(it->textTwo, it->theMatchedName, 0);
                   ofSetColor(255,255,255);
                   drawName(left);
                   drawName(right);
                   ofSetColor(50,255,50);
                   highlightConnection(*it);
                   break;
               } 
            else connectionMousedOver = FALSE;
        }  
            }
    ofSetColor(100,50,155); 
    

    //system(("open " + ofToDataPath(theFilename)).c_str());
    exit();
}
//-----------------------
void testApp::highlightConnection(foundPair &highlightedMatch){
    int offsetCurve = 200;
        ofNoFill();
        ofBezier(highlightedMatch.textOne.x+myFont.stringWidth(highlightedMatch.theMatchedName)+5, highlightedMatch.textOne.y, highlightedMatch.textOne.x+offsetCurve, highlightedMatch.textOne.y, 
                 highlightedMatch.textTwo.x-offsetCurve, highlightedMatch.textTwo.y, highlightedMatch.textTwo.x-3, highlightedMatch.textTwo.y);
        ofFill();
        ofCircle(highlightedMatch.textOne.x+myFont.stringWidth(highlightedMatch.theMatchedName)+5, highlightedMatch.textOne.y, 2);
        ofCircle(highlightedMatch.textTwo.x-4, highlightedMatch.textTwo.y, 2);
       
    
}
//-----------------------
void testApp::drawConnection(vector<foundPair> &foundMatches, bool _connectionMoused){
    int offsetCurve = 200;
    if(_connectionMoused) ofSetColor(50,255,50,50) ;
    for(vector<foundPair>::iterator iter = foundMatches.begin(); iter != foundMatches.end(); ++iter){
        highlightConnection(*iter);
        /*ofNoFill();
        ofBezier(iter->textOne.x+myFont.stringWidth(iter->theMatchedName)+5, iter->textOne.y, iter->textOne.x+offsetCurve, iter->textOne.y, 
                     iter->textTwo.x-offsetCurve, iter->textTwo.y, iter->textTwo.x-3, iter->textTwo.y);
        ofFill();
        ofCircle(iter->textOne.x+myFont.stringWidth(iter->theMatchedName)+5, iter->textOne.y, 1.5);
        ofCircle(iter->textTwo.x-3, iter->textTwo.y, 1.5);*/
        }
}
//-------------------------------------------
vector<foundPair> testApp::findConnection(vector<nameStruct> &textOne, vector<nameStruct> &textTwo, int _startItems, int _numItems, int _numItemsTwo){
    vector<foundPair>  _foundMatches;
    for(int tOne = _startItems; tOne< _startItems+_numItems; tOne++){
        for(int tTwo = _startItems; tTwo < _startItems + _numItemsTwo; tTwo++){
            if(textOne[tOne].theName.compare(textTwo[tTwo].theName)== 0){
                foundPair aConnection = foundPair(textOne[tOne].point, textTwo[tTwo].point ,textOne[tOne].theName) ;
                _foundMatches.push_back(aConnection);
                }
            }
        }
        return _foundMatches;
    }
//_______________
void testApp::drawName(nameStruct &text){
    myFont.drawString(text.theName, text.point.x, text.point.y);
}
//------------------------------------
void testApp::drawVizVector(vector<nameStruct> & _theVectortoDraw, int _startItems, int _numItems, int _fontSize,                
                                                bool _connectionMoused){
   
    if(_connectionMoused) {ofSetColor(150,150,150,20); _fontSize = 12;}
    else ofSetColor(255,255,255,50); 
    myFont.loadFont("Verdana.ttf", _fontSize, true, true, true);
    for(int s = _startItems; s < _startItems +_numItems; s++){
        drawName(_theVectortoDraw[s]);
        /*myFont.loadFont("Verdana.ttf", (int) ofClamp(_theVectortoDraw[s].count, fontSize , fontSize), true, true, true);
        myFont.drawString(_theVectortoDraw[s].theName, _theVectortoDraw[s].point.x, _theVectortoDraw[s].point.y);*/
    }
    /*/for(int d = _startItems; d< _startItems +_numItems; d++){
        for(int e = _startItems; e < _startItems + _numItems; e++){
            if(_theVectortoDraw[d].theName.compare(_theVectorDrawnTo[e].theName) == 0){
                        ofSetColor(0,50,255);
                        ofNoFill();
                        ofBezier(_theVectortoDraw[d].point.x+myFont.stringWidth(_theVectortoDraw[d].theName)+5, _theVectortoDraw[d].point.y, _theVectortoDraw[d].point.x+offsetCurve, _theVectortoDraw[d].point.y, 
                                 _theVectorDrawnTo[e].point.x-offsetCurve, _theVectorDrawnTo[e].point.y, _theVectorDrawnTo[e].point.x-3, _theVectorDrawnTo[e].point.y);
                        ofFill();
                        ofCircle(_theVectortoDraw[d].point.x+myFont.stringWidth(_theVectortoDraw[d].theName)+5, _theVectortoDraw[d].point.y, 1.5);
                        ofCircle(_theVectorDrawnTo[e].point.x-3, _theVectorDrawnTo[e].point.y, 1.5);
                                    }
                }
            }*/
        }

//----------------------------------------------
vector<nameStruct> testApp::createVisualizationVector(string pathToData, int flagPosition){
    vector<string>_baseText = cleanPunctuation(pathToData);
        //writeNamesToFile(_baseText, "clean" +pathToData);
    
    vector<string> foundProper = properNames(_baseText);
        //writeNamesToFile(foundProper, "proper" +pathToData);
   
    vector<string> makeUnique = uniqueNames(foundProper);
       // writeNamesToFile(makeUnique, "unique"+pathToData);
   
    vector<nameStruct> visualizationVector;
   
    for(vector<string>::iterator it = makeUnique.begin(); it != makeUnique.end(); it++){
        string _token = *it;
        nameStruct _theName = createNameStructs(_token, foundProper, flagPosition);
        visualizationVector.push_back(_theName);
    }
    
    float _minLocation = minLocation(visualizationVector);
    float _maxLocation = maxLocation(visualizationVector);
    
    for(int f = 0; f < visualizationVector.size(); f++){
        visualizationVector[f].setLocation(visualizationVector[f].point.x, ofMap(visualizationVector[f].point.y,_minLocation, 
                                                                                                                        _maxLocation,10.0, (float) ofGetScreenHeight()));
    }
    return visualizationVector;
}
//______________________________________
float testApp::maxLocation(vector<nameStruct> _structofNames){
    float _maxLocation = _structofNames[0].point.y;
    for(int i = 0; i<_structofNames.size(); i++){
        if(_structofNames[i].point.y > _maxLocation) _maxLocation = _structofNames[i].point.y;
    }
    return _maxLocation;
}
//______________________________________
float testApp::minLocation(vector<nameStruct> _structofNames){
    float _minLocation = _structofNames[0].point.y;
    for(int i = 0; i<_structofNames.size(); i++){
        if(_structofNames[i].point.y < _minLocation) _minLocation = _structofNames[i].point.y;
        }
    return _minLocation;
}
//_________________________
 nameStruct testApp::createNameStructs(string token, vector<string>_baseText, int flagPosition){
    //function should return nameStruct with a point, properName and appearances for sizing this can be called to create vector of structs
    
    int appearances =1;
    int sumOfLocations =0;
    float averagePlace =0.0;
    ofPoint aPoint;
    for(int j = 0; j< _baseText.size(); j++){
        
        int foundToken = icompare(token, _baseText[j]);
        if(foundToken == 0){ 
            appearances ++;
            sumOfLocations +=j;
        }
    }
    aPoint.y = (float) sumOfLocations/appearances;
     if(flagPosition ==0) aPoint.x = 0.125*(float) ofGetWidth();
    if(flagPosition ==1) aPoint.x = .875*(float) ofGetWidth();
     //3/8 factor is to divide the screen into 4 parts and center it in the second
    nameStruct _theName = nameStruct(aPoint, token, appearances);
    return _theName; 
}
//_________________________
void testApp::writeNamesToFile(vector<string> _names, string toFilePath){ 
    //writes a proper names vector to a file by converting to a string and placing in a buffer
    string writeName;
    ofBuffer tempBuffer;

    for( vector<string>::iterator q = _names.begin(); q !=_names.end(); q++)
    {
        writeName += *q + "\n";
        
    }
    tempBuffer.set(writeName.c_str(), writeName.size());
    ofBufferToFile(toFilePath, tempBuffer);
}
//__________________________
vector<string> testApp::cleanPunctuation(string filenname){ 
    //removes all common punctuation i.e. commas, periods, semicolons etc.
    ofBuffer file = ofBufferFromFile(filenname);
    string textAsString = file;
    
    for (int i = 0; i < textAsString.size(); i++){
        if (textAsString[i] == '.'){
            textAsString[i] = ' ';
        }
        if (textAsString[i] == '\n'){
            textAsString[i] = ' ';
        }
        if (textAsString[i] == '\t'){
            textAsString[i] = ' ';
        }
        if (textAsString[i] == ','){
            textAsString[i] = ' ';
        }
        if (textAsString[i] == '!'){
            textAsString[i] = ' ';
        }
        if (textAsString[i] == '?'){
            textAsString[i] = ' ';
        }
        if (textAsString[i] == ':'){
            textAsString[i] = ' ';
        }
        if (textAsString[i] == '\"'){
            textAsString[i] = ' ';
        }
        if (textAsString[i] == ';'){
            textAsString[i] = ' ';
        }
    }
    vector <string> words = ofSplitString(textAsString, " ", true, true);
    return words;
}
//--------------------------------------------------------------
int testApp::isCommonWord(string _testString, vector<string> _ignoreWords){
    //returns 0 if common word is found from a given vector or list of words to Ignore, returns 1 or -1 if no word can be found
    
    vector<string>::iterator g;
    int isCommon;
    for(int i =0; i<_ignoreWords.size(); i++){
        isCommon = icompare(_testString, _ignoreWords[i]);
        if(isCommon == 0) break;
        }
        return isCommon;
}
//--------------------------------------------------------------
vector<string> testApp::properNames(vector<string> _words){
    //returns vector of Proper(-ish) names by calling isCommonWord for each word in text file and pushing back in the names vector for unique words, also checks to make sure first letter is capitalized and that there is a second letter which is lowercase 
    
    vector<string> names;
    ofBuffer ignoreFile = ofBufferFromFile("ignoreList.txt");
    string ignoreString = ignoreFile;
    
    vector<string> ignoreWords = ofSplitString(ignoreString, "\n", true, true);
    //vector <string> :: iterator n;
   
    for(int i = 0; i< _words.size(); i++)
    {
        int isCommon = isCommonWord(_words[i], ignoreWords);
        //cout << isCommon;
        string testString = _words[i];
        if(isCommon != 0)
        {    
            char firstLetter = testString[0];
            char secondLetter = testString[1];
            locale loc;
            if(isupper(firstLetter, loc) && islower(secondLetter,loc)){ 
                names.push_back(testString);
            }
        }
    }
    return names;
}
//--------------------------------------------------------------
vector<string> testApp::uniqueNames(vector<string> _words){
    //create vector of unique tokens from given vector
    vector<string> _uniqueNames;
    _uniqueNames.push_back(_words[0]);
    
    for(int n = 0; n <  _words.size(); n++)
    {
        int isFound =isCommonWord(_words[n], _uniqueNames);
        if(isFound !=0) _uniqueNames.push_back(_words[n]);
        }
    return _uniqueNames;
}
//
void testApp::drawBackground(){
    ofTrueTypeFont bgFont;
    bgFont.loadFont("Cambria Italic.ttf", 20);
    //bgFont.drawString("Quran", <#float x#>, <#float y#>);
}
//--------------------------------------------------------------
void testApp::keyPressed(int key){

        if(key == 'n'){
        sort(vizQuran.begin(), vizQuran.end(), testApp::sortOnName);
        sort(vizNT.begin(), vizNT.end(), testApp::sortOnName);
        
            if(startItems + numItems > vizQuran.size() || startItems + numItems > vizNT.size()) startItems =0;
            else startItems += numItems;       
        }
    
    if(key == 'a'){
        sort(vizQuran.begin(), vizQuran.end(), testApp::sortOnAppearances);
        sort(vizNT.begin(), vizNT.end(), testApp::sortOnAppearances);
       
        if(startItems + numItems > vizQuran.size() || startItems + numItems > vizNT.size()) startItems =0;
        else startItems += numItems;  
    }
    
    if(key == '3'){
        startItems =0;
    }
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
    }

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
      
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}