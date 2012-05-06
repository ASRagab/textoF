#include "testApp.h"
#include "Poco/RegularExpression.h"
#include "Poco/UTF8String.h"
#include "Poco/String.h"
using Poco::RegularExpression;
using Poco::UTF8;
using Poco::icompare;

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
    //numItems = 100; 
   numItems = vizQuran.size();
    //numItemsTwo = 100; 
   numItemsTwo = vizNT.size();
    startItems = 0;
    startItemsTwo = 0;
    myFont.loadFont("Verdana.ttf", 20, true, false, true);
    bgFont.loadFont("Cambria Italic.ttf", 14, true, false, true);
    QandNT = findConnection(vizQuran, vizNT, startItems, startItemsTwo, numItems, numItemsTwo);
    bsaveScreen = FALSE;
    /*ofBuffer ntFile = ofBufferFromFile("newTestament.txt");
        string ntAsString = ntFile;
        RegularExpression myRegEx("[0-9]+:[0-9]+"); 
        RegularExpression::Match match;
        int found = myRegEx.subst(ntAsString," ", 0x10000000);
        myRegEx.subst(ntAsString, " ");
        ntFile.set(ntAsString.c_str(), ntAsString.size());
        ofBufferToFile("newTestament2.txt", ntFile);*/
        inputItems = "";
        saveVector.init(1,1, true);
}

//--------------------------------------------------------------
void testApp::update(){
QandNT = findConnection(vizQuran, vizNT, startItems, startItemsTwo, numItems, numItemsTwo);
    
}

//--------------------------------------------------------------
void testApp::draw(){
    if(bsaveScreen) {saveVector.begin();}
    
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    ofSetColor(255,255,255);
    bgFont.drawStringAsShapes("number of words\n to search for: " + inputItems, 25, 50);
    if(!inputItems.empty() && (ofToInt(inputItems) <= vizQuran.size() && ofToInt(inputItems) <= vizNT.size())){
    numItems = ofToInt(inputItems);
    numItemsTwo = ofToInt(inputItems);
        }
    ofSetColor(255,255,255,50);
    int fontSize = 9;
    string howToSort;
    if(ofGetKeyPressed() == OF_KEY_SHIFT) howToSort = "alphabetical";
    if(ofGetKeyPressed() == OF_KEY_CTRL) howToSort = "appearances";
    drawVizVector("Quran", vizQuran, howToSort, startItems, numItems, fontSize, connectionMousedOver); 
    drawVizVector("New Testament", vizNT, howToSort, startItemsTwo, numItemsTwo, fontSize,   
                                connectionMousedOver);
    ofSetColor(50,255,50,50);
    drawConnection(QandNT, connectionMousedOver);
        if(!QandNT.empty()){
            myFont.loadFont("Verdana.ttf", 20, true, false, true);
            for(vector<foundPair>::iterator it = QandNT.begin(); it < QandNT.end(); it++){
            if(((mouseX >= (int) it->textOne.x && mouseX <= (int) (it->textOne.x + myFont.stringWidth(it->theMatchedName))) && 
                (mouseY <= (int) it->textOne.y && mouseY >= (int)(it->textOne.y - myFont.stringHeight(it->theMatchedName)))) ||
               ((mouseX >= (int) it->textTwo.x && mouseX <= (int) (it->textTwo.x + myFont.stringWidth(it->theMatchedName))) && 
                (mouseY <= (int) it->textTwo.y && mouseY >= (int)(it->textTwo.y - myFont.stringHeight(it->theMatchedName))))){
                   connectionMousedOver = TRUE;
                   nameStruct left = nameStruct(it->textOne, it->theMatchedName,it->countOne );
                   nameStruct right = nameStruct(it->textTwo, it->theMatchedName, it->countTwo);
                   ofSetColor(255,255,255);
                   drawName(left);
                   drawName(right);
                   ofSetColor(50,255,50);
                   highlightConnection(*it);
                   drawConnectionFacts(left, right);
                   break;
               } 
            else connectionMousedOver = FALSE;
        }  
            }
    ofSetColor(100,50,155); 
    ofCircle(2000,2000, 200);
    if(bsaveScreen) {
        saveVector.end();     
        }
    //system(("open " + ofToDataPath(theFilename)).c_str());
    
}
//----------------------------------

//__________
void testApp::drawConnectionFacts(nameStruct _left, nameStruct _right){
    int alpha = 200;
    ofSetColor(0,0,0, alpha);
     ofFill();
    float factOffset = 200;
    float factOffsetRight = 150;
    float xAvg = (_left.point.x+_right.point.x)/2.0;
    float yAvg = (_left.point.y+_right.point.y)/2.0;
    ofSetColor(255,255,255, alpha);
    if(bsaveScreen){
    if(yAvg >= ofGetWindowHeight()/2.0){
bgFont.drawStringAsShapes("appears\n " + ofToString(_left.count)+ " times", _left.point.x-factOffset, _left.point.y);
bgFont.drawStringAsShapes("appears\n " + ofToString(_right.count)+ " times", _right.point.x+factOffsetRight, _right.point.y);
    }
    if(yAvg < ofGetWindowHeight()/2.0){
bgFont.drawStringAsShapes("appears \n" + ofToString(_left.count)+ " times", _left.point.x-factOffset, _left.point.y);
bgFont.drawStringAsShapes("appears \n" + ofToString(_right.count)+ " times", _right.point.x+factOffsetRight, _right.point.y);
    }
    }
    else{
        if(yAvg >= ofGetWindowHeight()/2.0){
            bgFont.drawString("appears\n " + ofToString(_left.count)+ " times", _left.point.x-factOffset, _left.point.y);
            bgFont.drawString("appears\n " + ofToString(_right.count)+ " times", _right.point.x+factOffsetRight, _right.point.y);
        }
        if(yAvg < ofGetWindowHeight()/2.0){
            bgFont.drawString("appears \n" + ofToString(_left.count)+ " times", _left.point.x-factOffset, _left.point.y);
            bgFont.drawString("appears \n" + ofToString(_right.count)+ " times", _right.point.x+factOffsetRight, _right.point.y);
        }
    }
    
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
                }
}
//-------------------------------------------
vector<foundPair> testApp::findConnection(vector<nameStruct> &textOne, vector<nameStruct> &textTwo, int _startItems, int _startItemsTwo, int _numItems, int _numItemsTwo){
    vector<foundPair>  _foundMatches;
    for(int tOne = _startItems; tOne< _startItems+_numItems; tOne++){
        for(int tTwo = _startItemsTwo; tTwo < _startItemsTwo + _numItemsTwo; tTwo++){
            if(textOne[tOne].theName.compare(textTwo[tTwo].theName)== 0){
                foundPair aConnection = foundPair(textOne[tOne].point, textTwo[tTwo].point ,textOne[tOne].theName, 
                                                  textOne[tOne].count, textTwo[tTwo].count) ;
                _foundMatches.push_back(aConnection);
                }
            }
        }
        return _foundMatches;
    }
//_______________
void testApp::drawName(nameStruct &text){
    if(bsaveScreen) myFont.drawStringAsShapes(text.theName, text.point.x, text.point.y);
    else myFont.drawString(text.theName, text.point.x, text.point.y);
}
//------------------------------------
void testApp::drawVizVector(string _vectorTitle, vector<nameStruct> & _theVectortoDraw, string _sort, int _startItems, int _numItems, int _fontSize, bool _connectionMoused){
    int top =25;
    ofSetColor(50,255,50);
    bgFont.loadFont("Cambria Italic.ttf", 14, true, false, true);
    bgFont.drawStringAsShapes(_vectorTitle, _theVectortoDraw[0].point.x, top);
    if(_connectionMoused) {ofSetColor(150,150,150,20); _fontSize = 12;}
    else ofSetColor(255,255,255,50); 
    myFont.loadFont("Verdana.ttf", _fontSize, true, false, true);
    for(int s = _startItems; s < _startItems +_numItems; s++){
    drawName(_theVectortoDraw[s]);
    }
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
        visualizationVector[f].setLocation(visualizationVector[f].point.x, ofMap(visualizationVector[f].point.y,_minLocation, _maxLocation, 75.0, (float) ofGetWindowHeight() - 25.0));
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
    ofPoint aPoint;
    for(int j = 0; j< _baseText.size(); j++){
        
        int foundToken = icompare(token, _baseText[j]);
        if(foundToken == 0){ 
            appearances ++;
            sumOfLocations +=j;
        }
    }
    aPoint.y = (float) sumOfLocations/appearances;
     if(flagPosition ==0) aPoint.x = 0.30*(float) ofGetWindowWidth();
    if(flagPosition ==1) aPoint.x = .70*(float) ofGetWindowWidth();
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
//---------------------------------------------
void testApp::drawBackground(){
    
}
//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if (inputItems.length() <= 4){
        if (key == '0') inputItems += "0";
        if (key == '1') inputItems += "1";
        if (key == '2') inputItems += "2";
        if (key == '3') inputItems += "3";
        if (key == '4') inputItems += "4";
        if (key == '5') inputItems += "5";
        if (key == '6') inputItems += "6";
        if (key == '7') inputItems += "7";
        if (key == '8') inputItems += "8";
        if (key == '9') inputItems += "9";
    }

    if (key == OF_KEY_DEL || key == OF_KEY_BACKSPACE){
        if (inputItems.size() > 0){
            inputItems.erase(inputItems.end()-1);
            }
        }

        if(key == OF_KEY_DOWN){
        sort(vizQuran.begin(), vizQuran.end(), testApp::sortOnName);
        sort(vizNT.begin(), vizNT.end(), testApp::sortOnName);
        
            if(startItems + numItems > vizQuran.size()) startItems =0;
            else startItems += numItems; 
            
            if(startItemsTwo + numItemsTwo > vizNT.size()) startItemsTwo = 0;
            else startItemsTwo += numItemsTwo;
        }
    
    if(key == OF_KEY_UP){
        sort(vizQuran.begin(), vizQuran.end(), testApp::sortOnAppearances);
        sort(vizNT.begin(), vizNT.end(), testApp::sortOnAppearances);
       
        if(startItems + numItems > vizQuran.size()) startItems =0;
        else startItems += numItems; 
        
        if(startItemsTwo + numItemsTwo > vizNT.size()) startItemsTwo = 0;
        else startItemsTwo += numItemsTwo;    
    }
    
    if(key == OF_KEY_RETURN){
        startItems =0;
        startItemsTwo = 0;
    }
    
    if(key =='s') {bsaveScreen = TRUE; saveVector.finish("QandNTMap.png", true);}
    else bsaveScreen = FALSE;

    
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