
#include "ofxAutoReloadedUrlShader.h"



ofxAutoReloadedUrlShader::ofxAutoReloadedUrlShader(){
	bWatchingFiles = false;
}

ofxAutoReloadedUrlShader::~ofxAutoReloadedUrlShader(){
	disableWatchFiles();
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
bool ofxAutoReloadedUrlShader::load(string shaderName )
{
	return load( shaderName + ".vert", shaderName + ".frag", shaderName + ".geom" );
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
bool ofxAutoReloadedUrlShader::load(string vertName, string fragName, string geomName)
{
	unload();
	
    ofShader::setGeometryOutputCount(geometryOutputCount);
    ofShader::setGeometryInputType(geometryInputType);
    ofShader::setGeometryOutputType(geometryOutputType);

    
	// hackety hack, clear errors or shader will fail to compile
	GLuint err = glGetError();
	
	lastTimeCheckMillis = ofGetElapsedTimeMillis();
	setMillisBetweenFileCheck( 2 * 1000 );
	enableWatchFiles();
	
	loadShaderNextFrame = false;

    vertexShaderUrl = vertName;
    fragmentShaderUrl = fragName;
    geometryShaderUrl = geomName;

    ofHttpResponse vertexShaderResponse = ofLoadURL(vertexShaderUrl);
    ofHttpResponse fragmentShaderResponse = ofLoadURL(fragmentShaderUrl);
    ofHttpResponse geometryShaderResponse = ofLoadURL(geometryShaderUrl);

    if( vertexShaderResponse.status == 200 && vertexShaderResponse.data.size() > 0 )
	{
		setupShaderFromSource(GL_VERTEX_SHADER, vertexShaderResponse.data);
	}

	if( fragmentShaderResponse.status == 200 && fragmentShaderResponse.data.size() > 0 )
	{
		setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderResponse.data);
	}

	#ifndef TARGET_OPENGLES
	if( geometryShaderResponse.status == 200 && geometryShaderResponse.data.size() > 0 )
	{
		setupShaderFromSource(GL_GEOMETRY_SHADER_EXT, geometryShaderResponse.data);
	}
	#endif

	bindDefaults();
	
	return linkProgram();
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofxAutoReloadedUrlShader::_update(ofEventArgs &e)
{
	if( loadShaderNextFrame )
	{
		reloadShaders();
		loadShaderNextFrame = false;
	}
	
	int currTime = ofGetElapsedTimeMillis();
	
	if( ((currTime - lastTimeCheckMillis) > millisBetweenFileCheck) &&
	   !loadShaderNextFrame )
	{
        loadShaderNextFrame = true;

		lastTimeCheckMillis = currTime;
	}
}


// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
bool ofxAutoReloadedUrlShader::reloadShaders()
{
	return load( vertexShaderUrl,  fragmentShaderUrl, geometryShaderUrl );
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofxAutoReloadedUrlShader::enableWatchFiles()
{
	if(!bWatchingFiles){
		ofAddListener(ofEvents().update, this, &ofxAutoReloadedUrlShader::_update );
		bWatchingFiles = true;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofxAutoReloadedUrlShader::disableWatchFiles()
{
	if(bWatchingFiles){
		ofRemoveListener(ofEvents().update, this, &ofxAutoReloadedUrlShader::_update );
		bWatchingFiles = false;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------
//
void ofxAutoReloadedUrlShader::setMillisBetweenFileCheck( int _millis )
{
	millisBetweenFileCheck = _millis;
}

//--------------------------------------------------------------
void ofxAutoReloadedUrlShader::setGeometryInputType(GLenum type) {
    ofShader::setGeometryInputType(type);
    geometryInputType = type;
}

//--------------------------------------------------------------
void ofxAutoReloadedUrlShader::setGeometryOutputType(GLenum type) {
    ofShader::setGeometryOutputType(type);
    geometryOutputType = type;
}

//--------------------------------------------------------------
void ofxAutoReloadedUrlShader::setGeometryOutputCount(int count) {
    ofShader::setGeometryOutputCount(count);
    geometryOutputCount = count;
}
