#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos; // 新增：摄像机位置

void main()
{
	// 1. 环境光
	float ambientStrength = 0.3; // 卡通风格通常需要更强的环境光
	vec3 ambient = ambientStrength * lightColor;

	// 2. 漫反射 - 使用离散化处理
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);

	// 离散化处理 - 产生阶梯状光照
	float celLevels = 4.0; // 色阶数量
	diff = floor(diff * celLevels) / celLevels;

	vec3 diffuse = diff * lightColor;

	// 3. 轮廓光 - 卡通效果的边缘高光
	vec3 viewDir = normalize(viewPos - FragPos);
	float edge = dot(norm, viewDir);
	if (edge < 0.2) { // 边缘检测阈值
		edge = 0.0;   // 边缘处变暗
	} else {
		edge = 1.0;
	}

	// 4. 高光 - 离散化的镜面反射
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	spec = floor(spec * 2.0) / 2.0; // 只有两阶高光

	// 5. 组合结果
	vec3 result = (ambient + diffuse + spec * 0.5) * objectColor * edge;
	FragColor = vec4(result, 1.0);
}