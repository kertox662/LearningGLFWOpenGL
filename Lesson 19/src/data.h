glm::vec3 dirs[6];
dirs[0] = glm::vec3(0.0f,0.0f,-3.0f);
dirs[1] = glm::vec3(0.0f,0.0f, 3.0f);
dirs[2] = glm::vec3(3.0f,0.0f, 0.0f);
dirs[3] = glm::vec3(-3.0f,0.0f,0.0f);
dirs[4] = glm::vec3(0.0f,3.0f, 0.0f);
dirs[5] = glm::vec3(0.0f,-3.0f,0.0f);

Material mats[6];
mats[0].shininess = 32.0f;
mats[1].shininess = 128.0f;
mats[2].shininess = 64.0f;
mats[3].shininess = 32.0f;
mats[4].shininess = 32.0f;
mats[5].shininess = 256.0f;