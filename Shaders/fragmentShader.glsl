#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 vertexColor;

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;
	//
    //// diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
	//
    vec3 result = (ambient + diffuse) * objectColor;
	if(gl_FrontFacing){

	    FragColor = vec4(vec3(result)*vertexColor,1.0);	
	}
	else{
		FragColor = vec4(1.0,0.0,0.0,1.0);

	}
}
