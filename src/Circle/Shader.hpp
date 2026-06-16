#include <fstream>
#include <sstream>


class Shader {
public:
  void loadShader(const char *vertexShaderPath, const char *fragmentShaderPath);

  void compileShader();
  void use();

  private:

  unsigned int ID = 0;
  std::string vertexCode, fragmentCode;
  const char *vertexShaderCode;
  const char *fragmentShaderCode;
  int success;
  char infoLog[512];

    
};
void Shader::loadShader(const char* vertexShaderPath, const char* fragmentShaderPath) {

  std::ifstream vertexShaderFile, fragmentShaderFile;


  try {
    vertexShaderFile.open(vertexShaderPath);
    fragmentShaderFile.open(fragmentShaderPath);

    std::stringstream vertexShaderStream, fragmentShaderStream;
    vertexShaderStream << vertexShaderFile.rdbuf();
    fragmentShaderStream << fragmentShaderFile.rdbuf();
    vertexCode = vertexShaderStream.str();
    fragmentCode = fragmentShaderStream.str();

    vertexShaderFile.close();
    fragmentShaderFile.close();

  } catch (const std::exception &) {
  }

 vertexShaderCode = vertexCode.c_str();
 fragmentShaderCode = fragmentCode.c_str();
}

void Shader::compileShader() { 
    unsigned int vertex, fragment;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

      if (!success) {
      glGetShaderInfoLog(vertex, 512, NULL, infoLog);
      std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl;
      std::cerr << infoLog << std::endl;
      throw std::runtime_error("Vertex shader compilation failed");
    }


    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(fragment, 512, NULL, infoLog);
      std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << std::endl;
      std::cerr << infoLog << std::endl;
      glDeleteShader(vertex);
      throw std::runtime_error("Fragment shader compilation failed");
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(ID, 512, NULL, infoLog);
      std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << std::endl;
      std::cerr << infoLog << std::endl;
      glDeleteShader(vertex);
      glDeleteShader(fragment);
      throw std::runtime_error("Shader program linking failed");
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}


void Shader::use() { glUseProgram(ID); }
