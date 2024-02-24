#version 330 core
uniform float transparency;

vec3 lightColor;
vec3 color;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform mat4 lightSpaceMatrix;
uniform samplerCube depthMap;
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;
uniform float near_plane;
uniform float far_plane;
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}
vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1),
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);
float ShadowCalculation(vec3 fragPos)
{
    //
    /*
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(depthMap, projCoords.xy).r;
    closestDepth = LinearizeDepth(closestDepth) / far_plane;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    currentDepth = LinearizeDepth(currentDepth) / far_plane;
    // check whether current frag pos is in shadow

    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.0001);
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(depthMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = LinearizeDepth(texture(depthMap, projCoords.xy + vec2(x, y) * texelSize).r)/far_plane;
            shadow += (currentDepth - bias)> pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    if(projCoords.z > 1.0)
            shadow = 0.0;

    return shadow;*/


    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);
    //float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
    float shadow  = 0.0;
    float bias   = 0.04;
    int samples  = 20;
    float viewDistance = length(viewPos - fragPos);

    float diskRadius = (1.0 + (viewDistance / far_plane)) / far_plane;

    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(depthMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;

        closestDepth *= far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
    return shadow;



}
void main(void)

{

    float ambientStrength = 0.05;
    //float specularStrength = 0.2;
    lightColor = vec3(1.0,1.0,1.0);
    color = vec3(1.0f,1.0f,1.0f);
   // color = texture(color, fs_in.TexCoords).rgb;
    vec3 ambient = ambientStrength * lightColor;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir,normal), 0.0);
    vec3 diffuse = (diff * 1.0f )* lightColor;

    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = (spec * 0.1f)* lightColor;
    // calculate shadow
    float shadow =  ShadowCalculation(fs_in.FragPos);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    gl_FragColor = vec4(lighting, 1.0);
    //float depth = LinearizeDepth(gl_FragCoord.z) / far_plane; // divide by far for demonstration
    //gl_FragColor = vec4(vec3(depth), 1.0);
    //float depthValue = texture(depthMap, TexCoords).r;
    //float depthValue = texture(depthMap, TexCoords).r;
    //gl_FragColor = vec4(vec3(depthValue), 1.0);
    //gl_FragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0); // perspective
;
}
