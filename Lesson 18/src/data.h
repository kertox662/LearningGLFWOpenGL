

glm::vec3 dirs[6];
dirs[0] = glm::vec3(0.0f,0.0f,-3.0f);
dirs[1] = glm::vec3(0.0f,0.0f, 3.0f);
dirs[2] = glm::vec3(3.0f,0.0f, 0.0f);
dirs[3] = glm::vec3(-3.0f,0.0f,0.0f);
dirs[4] = glm::vec3(0.0f,3.0f, 0.0f);
dirs[5] = glm::vec3(0.0f,-3.0f,0.0f);

Material mats[6];
mats[0].ambient = glm::vec3(1.0f, 0.4f, 0.35f);
mats[0].diffuse = glm::vec3(0.1f, 0.8f, 0.1f);
mats[0].specular = glm::vec3(1.0f, 1.0f, 1.0f);
mats[0].shininess = 32.0f;

mats[1].ambient = glm::vec3(0.1f, 0.1f, 0.6f);
mats[1].diffuse = glm::vec3(0.1f, 0.1f, 0.6f);
mats[1].specular = glm::vec3(1.0f, 1.0f, 1.0f);
mats[1].shininess = 128.0f;

mats[2].ambient = glm::vec3(1.0f, 0.1f, 0.55f);
mats[2].diffuse = glm::vec3(1.0f, 0.8f, 0.6f);
mats[2].specular = glm::vec3(1.0f, 1.0f, 1.0f);
mats[2].shininess = 64.0f;

mats[3].ambient = glm::vec3(0.0f, 0.2f, 0.35f);
mats[3].diffuse = glm::vec3(0.1f, 0.4f, 0.7f);
mats[3].specular = glm::vec3(1.0f, 1.0f, 1.0f);
mats[3].shininess = 32.0f;

mats[4].ambient = glm::vec3(1.0f, 0.1f, 0.1f);
mats[4].diffuse = glm::vec3(0.0f, 0.0f, 0.3f);
mats[4].specular = glm::vec3(0.4f, 0.4f, 0.4f);
mats[4].shininess = 32.0f;

mats[5].ambient = glm::vec3(1.0f, 0.8f, 0.5f);
mats[5].diffuse = glm::vec3(0.0f, 0.0f, 0.1f);
mats[5].specular = glm::vec3(1.0f, 1.0f, 1.0f);
mats[5].shininess = 256.0f;