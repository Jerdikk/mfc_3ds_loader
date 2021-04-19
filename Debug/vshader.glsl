#version 150

in  vec4 vPosition;
in  vec4 vNormal;

uniform mat4 p; 
uniform mat4 mv;
uniform vec4 lightPosition;
uniform vec3 uColor;

out vec4 color;

float shininess;
vec4 ambientProduct;
vec4 diffuseProduct;
vec4 specularProduct;
vec4 mvPosition;

void main() 
{
  //initialize variables
  shininess = 5.0;
  ambientProduct = vec4(0.2 * uColor, 1);
  diffuseProduct = vec4(0.8 * uColor,1);
  specularProduct = vec4(0.3);

  //Transform the point
  mvPosition = mv*vPosition; 
  gl_Position = p*mvPosition; 

  //Set up Normal, Light, Eye and Half vectors
  vec3 N = normalize((mv*vNormal).xyz);
  vec3 L = normalize(lightPosition.xyz - mvPosition.xyz);
  if (lightPosition.w == 0.0) L = normalize(lightPosition.xyz);
  vec3 E = -normalize(mvPosition.xyz);
  vec3 H = normalize(L+E); 

  //Calculate diffuse coefficient
  float Kd = max(dot(L,N), 0.0);

  //Calculate Blinn-Phong specular coefficient
  float Ks = pow(max(dot(N,H), 0.0), shininess);

  //Calculate lit colour for this pixel
  color =  Kd * diffuseProduct + Ks * specularProduct + ambientProduct;
}