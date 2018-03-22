#include "Polyhedron.h"

Polyhedron::Polyhedron() {
	wireframe = false;
}

//color constructor
Polyhedron::Polyhedron(vec4 d, vec4 s, vec4 a) {
	wireframe = false;
	diff = d;
	spec = s;
	ambi = a;
}

void Polyhedron::init() {
	index = 0;
	//buildCube();

	//get buffers for attributes and indices
	glGenBuffers(1, &VBO);
	assert((program = InitShader("vshader_basic.glsl", "fshader_basic.glsl")) != -1);
	glUseProgram(program);  //make it the active one

	//get vData
	assert((vPosition = glGetAttribLocation(program, "vPosition")) != -1);
	assert((vNormal = glGetAttribLocation(program, "vNormal")) != -1);


	//get light location
	diffuse_loc = glGetUniformLocation(program, "matDiffuse");
	spec_loc = glGetUniformLocation(program, "matSpecular");
	ambient_loc = glGetUniformLocation(program, "matAmbient");
	alpha_loc = glGetUniformLocation(program, "matAlpha");

	//get the location of the model matrix
	assert((mmLoc = glGetUniformLocation(program, "model_matrix")) != -1);
	assert((cmLoc = glGetUniformLocation(program, "camera_matrix")) != -1);
	assert((pmLoc = glGetUniformLocation(program, "proj_matrix")) != -1);

	//cout << "NORMAL SIZE: " << normals.size() << "\n";
	//cout << "POINTS SIZE: " << points.size() << "\n";

	//put the data on the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points)*points.size() + sizeof(normals)*normals.size(), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points)*points.size(), &points[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points)*points.size(), sizeof(normals)*normals.size(), &normals[0]);

	//set up stuff for the body of the Polyhedron
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); //make this VAO active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //associate the VBO with the active VAO

	glEnableVertexAttribArray(vPosition);  //enable it
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)*points.size()));
}

void Polyhedron::textureInit(Texture t) {
	index = 0;
	//buildCube();

	
	//get buffers for attributes and indices
	glGenBuffers(1, &VBO);
	assert((program = InitShader("vshader_texture.glsl", "fshader_texture.glsl"))!=-1);
	glUseProgram(program);  //make it the active one
	
	//get vData
	assert((vPosition = glGetAttribLocation(program, "vPosition")) != -1);
	assert((vNormal = glGetAttribLocation(program, "vNormal"))!=-1);
	assert((vTex = glGetAttribLocation(program, "vTexCoord"))!=-1);


	//get light location
	diffuse_loc = glGetUniformLocation(program, "matDiffuse");
	spec_loc = glGetUniformLocation(program, "matSpecular");
	ambient_loc = glGetUniformLocation(program, "matAmbient");
	alpha_loc = glGetUniformLocation(program, "matAlpha");

	//get the location of the model matrix
	assert((mmLoc = glGetUniformLocation(program, "model_matrix")) != -1);
	assert((cmLoc = glGetUniformLocation(program, "camera_matrix")) != -1);
	assert((pmLoc = glGetUniformLocation(program, "proj_matrix")) != -1);
	assert((texLoc = glGetUniformLocation(program, "textureID")) != -1);

	//put the data on the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points)*points.size() + sizeof(normals)*normals.size() + sizeof(textureCoords)*textureCoords.size(), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points)*points.size(), &points[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points)*points.size(), sizeof(normals)*normals.size(), &normals[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points)*points.size()+ sizeof(normals)*normals.size(), sizeof(textureCoords)*textureCoords.size(), &textureCoords[0]);

	//set up stuff for the body of the Polyhedron
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); //make this VAO active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //associate the VBO with the active VAO

	glEnableVertexAttribArray(vPosition);  //enable it
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)*points.size()));

	glEnableVertexAttribArray(vTex);
	glVertexAttribPointer(vTex, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)*points.size()*2));

	///load up texture
	int x = t.width;
	int y = t.height;

	//cout << t.isLoaded() << endl;
	

	if(!t.isLoaded()) {
		t.setImage(ppmRead(t.name, &x, &y));
		//cout << "loading texture" << t.name << endl;
		//cout << t.isLoaded() << endl;
	}

	//GLubyte * image = ppmRead(t.name, &x, &y);
	GLubyte * image = t.getImage();
	
	
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

}

Polyhedron::~Polyhedron(){

}

//----------------------------------------------------------------------------

void Polyhedron::draw(Camera cam, vector<Light*> lights){
	//Draw the Polyhedron bod
	glBindVertexArray(VAO);
	glUseProgram(program);  //also switch to using this shader program
	glUniformMatrix4fv(mmLoc, 1, GL_TRUE,modelmatrix);
	glUniformMatrix4fv(cmLoc, 1, GL_TRUE,cam.cameraMatrix);
	glUniformMatrix4fv(pmLoc, 1, GL_TRUE,cam.projection);
	glUniform4fv(diffuse_loc, 1, diff);
	glUniform4fv(spec_loc, 1, spec);	
	glUniform4fv(ambient_loc, 1, ambi);
	glUniform1f(alpha_loc, 100);
	glUniform1i(texLoc, 0);

	//load texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);


	//load sun
	GLuint light_loc = glGetUniformLocation(program, "SunlightPos");
	glUniform4fv(light_loc, 1, lights[0]->getPosition());
	GLuint ambient_loc2 = glGetUniformLocation(program, "SunlightAmbient");
	glUniform4fv(ambient_loc2, 1, lights[0]->getAmbient());
	GLuint diffuse_loc2 = glGetUniformLocation(program, "SunlightDiffuse");
	glUniform4fv(diffuse_loc2, 1, lights[0]->getDiffuse());
	GLuint specular_loc2 = glGetUniformLocation(program, "SunlightSpecular");
	glUniform4fv(specular_loc2, 1, lights[0]->getSpecular());
	//load flashlight
	GLuint light_locFlash = glGetUniformLocation(program, "FlashlightPos");
	glUniform4fv(light_locFlash, 1, lights[1]->getPosition());
	GLuint ambient_loc2Flash = glGetUniformLocation(program, "FlashlightAmbient");
	glUniform4fv(ambient_loc2Flash, 1, lights[1]->getAmbient());
	GLuint diffuse_loc2Flash = glGetUniformLocation(program, "FlashlightDiffuse");
	glUniform4fv(diffuse_loc2Flash, 1, lights[1]->getDiffuse());
	GLuint specular_loc2Flash = glGetUniformLocation(program, "FlashlightSpecular");
	glUniform4fv(specular_loc2Flash, 1, lights[1]->getSpecular());
	GLuint flashlight = glGetUniformLocation(program, "flashlight");
	glUniform1i(flashlight, lights[1]->isOn());

	//if(wireframe) {
		//glDrawArrays(GL_TRIANGLES, 0, points.size()/2);
		//glLineWidth(3);
		//for(int i=points.size()/2; i<points.size(); i += 3)
			//glDrawArrays(GL_LINE_LOOP, i, 3);
	//} else {
		glDrawArrays(GL_TRIANGLES, 0, points.size());
	//}
}

void Polyhedron::updateTexture(Texture t) {
	int x = t.width;
	int y= t.height;
	GLubyte *image0 = ppmRead(t.name, &x, &y);
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB,
	GL_UNSIGNED_BYTE, image0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

}

void Polyhedron::calcNormals() {
	for(int i=0; i<points.size(); i+=3) {
		vec3 N = normalize(cross(points[i+1]-points[i],points[i+2]-points[i]));
		normals.push_back(N); normals.push_back(N); normals.push_back(N);
	}
}

void Polyhedron::buildCube() {
	makeQuad(1,0,3,2);
	makeQuad(2,3,7,6);
	makeQuad(3,0,4,7);
	makeQuad(6,5,1,2);
	makeQuad(4,5,6,7);
	makeQuad(5,4,0,1);
}

void Polyhedron::makeQuad(GLuint a, GLuint b, GLuint c, GLuint d) {
	vec4 data[] = {vec4(-0.5,-0.5,0.5,1.0),vec4(-0.5,0.5,0.5,1.0),vec4(0.5,0.5,0.5,1.0),vec4(0.5,-0.5,0.5,1.0),
				   vec4(-0.5,-0.5,-0.5,1.0),vec4(-0.5,0.5,-0.5,1.0),vec4(0.5,0.5,-0.5,1.0),vec4(0.5,-0.5,-0.5,1.0)};

	vec3 N = normalize(cross(data[b]-data[a], data[c]-data[a]));
	addVert(data[a]); normals.push_back(N); textureCoords.push_back(vec2(1,1));
	addVert(data[b]); normals.push_back(N); textureCoords.push_back(vec2(0,0));
	addVert(data[c]); normals.push_back(N); textureCoords.push_back(vec2(1,0));

	N = normalize(cross(data[c]-data[a], data[d]-data[a]));
	addVert(data[c]); normals.push_back(N); textureCoords.push_back(vec2(0,0));
	addVert(data[d]); normals.push_back(N); textureCoords.push_back(vec2(1,1));
	addVert(data[a]); normals.push_back(N); textureCoords.push_back(vec2(0,1));
}

void Polyhedron::addVert(vec4 v) {
	points.push_back(v);

}

void Polyhedron::addVert(vec4 v, vec4 c) {
	points.push_back(v);
	colors.push_back(c);
}

void Polyhedron::loadSmf(string filename) {
	if(!filename.compare("polyhedron")){
		buildPolyhedron();
	} else if(!filename.compare("cube")) {
		buildCube();
	} else {
		//load file 
		ifstream infile(filename.c_str());
		string fin;

		//read
		while (getline(infile, fin)) {
			istringstream iss(fin);
			//split line
			vector<string> tokens;
			copy(istream_iterator<string>(iss),
				istream_iterator<string>(),
				back_inserter(tokens));

			if(!tokens[0].compare("v")){
				vertices.push_back(vec4(stof(tokens[1]), stof(tokens[2]), stof(tokens[3]), 1));
			} else if(!tokens[0].compare("f")) {
				points.push_back(vertices[stoi(tokens[1])-1]);
				points.push_back(vertices[stoi(tokens[2])-1]);
				points.push_back(vertices[stoi(tokens[3])-1]);
			}
		}
		calcNormals();
	}
}

void Polyhedron::loadObj(string filename, float scale) {
	//load file 
	ifstream infile(filename.c_str());
	string fin;
	int count = 1;
	//read
	while (getline(infile, fin)) {
		
		istringstream iss(fin);
		//split line
		vector<string> tokens;
		copy(istream_iterator<string>(iss),
			istream_iterator<string>(),
			back_inserter(tokens));
		if (tokens.size() > 0 && !tokens[0].compare("v")) {
			vertices.push_back(vec4(stof(tokens[1]) * scale, stof(tokens[2]) * scale, stof(tokens[3]) * scale, 1));
		} else if (tokens.size() > 0 && !tokens[0].compare("vn")) {
			normalVerts.push_back(vec3(stof(tokens[1]), stof(tokens[2]), stof(tokens[3])));
		} else if (tokens.size() > 0 && !tokens[0].compare("vt")) {
			textureVerts.push_back(vec2(stof(tokens[1]), stof(tokens[2])));
		} else if (tokens.size() > 0 && !tokens[0].compare("f")) {

			int f[3];
			int t[3];
			int n[3];

			for(int i=0; i<3; i++) {
				if (tokens[i+1].find("/") != std::string::npos) {
					//point
					f[i] = stoi(tokens[i+1].substr(0, tokens[i+1].find("/")))-1;
					points.push_back(vertices[f[i]]);
					tokens[i+1] = tokens[i+1].substr(tokens[i+1].find("/")+1);
					//texture
					t[i] = stoi(tokens[i+1].substr(0, tokens[i+1].find("/")))-1;
					textureCoords.push_back(vec2(textureVerts[t[i]].x,-1* textureVerts[t[i]].y));
					tokens[i+1] = tokens[i+1].substr(tokens[i+1].find("/")+1);
					//normal
					n[i] = stoi(tokens[i+1])-1;
					normals.push_back(normalVerts[n[i]]);
				} else points.push_back(vertices[stoi(tokens[i+1]) - 1]);
			}
		}
		count++;
	}
	//calcNormals();
}

vec4 Polyhedron::randomColor() {
	return vec4(randomFloat()+.01, randomFloat()+.1, randomFloat()+.04, 1.0);
}

GLfloat Polyhedron::randomFloat() {
	return ((GLfloat) rand() / (RAND_MAX));
}


void Polyhedron::makeWireframe() {
	wireframe = true;
	//make wireframe
	int s = points.size();
	for(int i=0; i<s; i++) {
		points.push_back(points[i]+vec4(.001*abs(points[i].x)/points[i].x,
			.001*abs(points[i].y)/points[i].y,
			.001*abs(points[i].z)/points[i].z,0));
		colors.push_back(vec4(0,0,0,1));
	}
}



void Polyhedron::buildPolyhedron() {
	vec4 c = randomColor();
	points.push_back(vec4(0.5,0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(0.5,-0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-0.5,-1,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-0.5,0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(0.5,0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-0.5,-1,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(0.5,0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(0.5,1,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,0.5,-1,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(0.5,1,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,1,-0.5,1)); colors.push_back(c);
	c = randomColor();
	
	points.push_back(vec4(0.5,0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(0.5,1,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(1,.5,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(0.5,0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(1,0.5,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(1,-0.5,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(0.5,0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(0.5,-0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(1,-0.5,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(0.5,-1,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(0.5,-0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(1,-0.5,-0.5,1)); colors.push_back(c);
	c = randomColor();

	points.push_back(vec4(0.5,-0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(0.5,-1,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-1,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(0.5,-0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-1,-0.5,1)); colors.push_back(c);
	c = randomColor();
		
	points.push_back(vec4(-0.5,0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,1,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(-1,.5,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-0.5,0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(-1,0.5,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(-1,-0.5,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-0.5,0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(-1,-0.5,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-0.5,-1,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(-1,-0.5,-0.5,1)); colors.push_back(c);
	c = randomColor();

	//////////////////////////////

	points.push_back(vec4(0.5,0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(0.5,-0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-0.5,1,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-0.5,0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(0.5,0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-0.5,1,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(0.5,0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(0.5,1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,0.5,1,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(0.5,1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,1,0.5,1)); colors.push_back(c);
	c = randomColor();
	
	points.push_back(vec4(0.5,0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(0.5,1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(1,.5,0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(0.5,0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(1,0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(1,-0.5,0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(0.5,0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(0.5,-0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(1,-0.5,0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(0.5,-1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(0.5,-0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(1,-0.5,0.5,1)); colors.push_back(c);
	c = randomColor();

	points.push_back(vec4(0.5,-0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(0.5,-1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-1,0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(0.5,-0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-1,0.5,1)); colors.push_back(c);
	c = randomColor();
		
	points.push_back(vec4(-0.5,0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-1,.5,0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-0.5,0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(-1,0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-1,-0.5,0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-0.5,0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(-1,-0.5,0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-0.5,-1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(-1,-0.5,0.5,1)); colors.push_back(c);
	c = randomColor();


	/////////////////////

	points.push_back(vec4(-1,0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-1,-0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-1,-0.5,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-1,0.5,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(-1,0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-1,-0.5,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-1,0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-1,0.5,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-0.5,1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-1,0.5,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,1,-0.5,1)); colors.push_back(c);
	c = randomColor();

	points.push_back(vec4(-1,-0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-1,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-1,-0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-1,-0.5,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-1,-0.5,1)); colors.push_back(c);
	c = randomColor();

	////////////////////////

	points.push_back(vec4(1,0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(1,-0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(1,-0.5,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(1,0.5,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(1,0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(1,-0.5,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(1,0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(0.5,1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(1,0.5,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(0.5,1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(1,0.5,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(0.5,1,-0.5,1)); colors.push_back(c);
	c = randomColor();

	points.push_back(vec4(1,-0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(0.5,-1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(0.5,-1,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(1,-0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(1,-0.5,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(0.5,-1,-0.5,1)); colors.push_back(c);
	c = randomColor();

	//////////////////top

	points.push_back(vec4(0.5,-1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(0.5,-1,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-1,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-0.5,-1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(0.5,-1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-1,-0.5,1)); colors.push_back(c);
	c = randomColor();

	points.push_back(vec4(0.5,1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(0.5,1,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,1,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-0.5,1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(0.5,1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,1,-0.5,1)); colors.push_back(c);
	c = randomColor();

	
}
