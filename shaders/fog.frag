precision mediump float;

varying vec3 fragPos;
varying vec3 fragNormal;
varying vec4 fragColor;

uniform vec3 viewPos;
uniform vec3 fogColor;
uniform float fogStart;
uniform float fogEnd;

void main()
{
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));
    
    float diff = max(dot(normalize(fragNormal), lightDir), 0.2);
    
    vec3 color = fragColor.rgb * diff;

    float dist = length(viewPos - fragPos);
    float fogFactor = clamp((fogEnd - dist) / (fogEnd - fogStart), 0.0, 1.0);

    vec3 result = mix(fogColor, color, fogFactor);
    gl_FragColor = vec4(result, 1.0);
}