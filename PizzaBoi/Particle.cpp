#include "Particle.h"


Particle::Particle(int s) {
	speed = .1;
	size = s;
	
	for(int i=0; i<size; i++) {
		mass.push_back(1);
		positions.push_back(vec4(2*((float)rand()/RAND_MAX)-1,2*((float)rand()/RAND_MAX)-1,2*((float)rand()/RAND_MAX)-1,1));
		velocities.push_back(speed*vec4(2*((float)rand()/RAND_MAX)-1,2*((float)rand()/RAND_MAX)-1,2*((float)rand()/RAND_MAX)-1,0));
		colors.push_back(vec4(2*((float)rand()/RAND_MAX)-1,2*((float)rand()/RAND_MAX)-1,2*((float)rand()/RAND_MAX)-1,1));
	}
	
	//loadObj("./objects/boundCow.obj", .4);
}


Particle::~Particle(){

}

void Particle::draw(Camera cam, vector<Light*>) {
	glBindVertexArray(VAO);
	glUseProgram(program);

	glUniformMatrix4fv(mmLoc, 1, GL_TRUE,modelmatrix);
	glUniformMatrix4fv(cmLoc, 1, GL_TRUE,cam.cameraMatrix);
	glUniformMatrix4fv(pmLoc, 1, GL_TRUE,cam.projection);

	glPointSize(5.0);
	glDrawArrays(GL_POINTS, 0, positions.size());

}

void Particle::init() {
	//get buffers for attributes and indices
	glGenBuffers(1, &VBO);

	assert((program = InitShader("vshader0.glsl", "fshader0.glsl"))!=-1);
	glUseProgram(program);  //make it the active one

	//put the data on the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions)*positions.size() + sizeof(colors)*colors.size()+ sizeof(velocities)*velocities.size(), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions)*positions.size(), &positions[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions)*positions.size(), sizeof(colors)*colors.size(), &colors[0]);

	//set up stuff for the body of the Polyhedron
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); //make this VAO active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //associate the VBO with the active VAO


	assert((vPosition = glGetAttribLocation(program, "vPosition")) != -1);
	glEnableVertexAttribArray(vPosition);  //enable it
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//get the location of the uniform color in the shader
	assert((vColor = glGetAttribLocation(program, "vColor"))!=-1);
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(positions)*positions.size()));

	//get the location of the model matrix
	assert((mmLoc = glGetUniformLocation(program, "model_matrix")) != -1);
	assert((cmLoc = glGetUniformLocation(program, "camera_matrix")) != -1);
	assert((pmLoc = glGetUniformLocation(program, "proj_matrix")) != -1);
	
}


void Particle::loadObj(string filename, float scale) {
	//load file 
	ifstream infile(filename.c_str());
	string fin;
	int count = 0;
	//read
	while (getline(infile, fin)) {
		istringstream iss(fin);
		//split line
		vector<string> tokens;
		copy(istream_iterator<string>(iss),
			istream_iterator<string>(),
			back_inserter(tokens));
		if (tokens.size() > 0 && !tokens[0].compare("v")) {
			positions.push_back(vec4(stof(tokens[1]) * scale, stof(tokens[2]) * scale, stof(tokens[3]) * scale, 1));
			velocities.push_back(vec4(stof(tokens[1]) * speed, stof(tokens[2]) * speed, stof(tokens[3]) * speed, 0));
			colors.push_back(vec4(2*((float)rand()/RAND_MAX)-1,2*((float)rand()/RAND_MAX)-1,2*((float)rand()/RAND_MAX)-1,1));
			count++;
		}
	}
	size = count;
}


void Particle::updateTexture(Texture t) {

}


void Particle::idle() {
	float dt = 0.0001;
	for(int i=0; i<size;i++){
		//positions[i] += .005;
		positions[i] += velocities[i];
	}

	glBindVertexArray(VAO); //make this VAO active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //associate the VBO with the active VAO
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions)*positions.size(), &positions[0]);

}
