#pragma once

#include "ofMain.h"

class ofxAutoReloadedUrlShader : public ofShader
{
	
  public:
	ofxAutoReloadedUrlShader();
	~ofxAutoReloadedUrlShader();
	
	// override the initialisation functions
    bool load(string shaderName );
	bool load(string vertName, string fragName, string geomName = "");
	
	bool reloadShaders();
	
	void enableWatchFiles();
	void disableWatchFiles();
		
	void setMillisBetweenFileCheck( int _millis );
	
	void _update(ofEventArgs &e);
    
    void setGeometryInputType(GLenum type);
    void setGeometryOutputType(GLenum type);
    void setGeometryOutputCount(int count);
	
private:
	
	bool bWatchingFiles;
	bool filesChanged();
	
	bool loadShaderNextFrame;
	
	std::time_t getLastModified( ofFile& _file );
	
	int lastTimeCheckMillis;
	int millisBetweenFileCheck;
	
	string vertexShaderUrl;
	string fragmentShaderUrl;
	string geometryShaderUrl;
	
	vector< std::time_t > fileChangedTimes;
    
    GLenum geometryInputType, geometryOutputType;
    int geometryOutputCount;
};


