#include "header/io.hpp"

bool IO::validYN(string msg){
  string input;
  cout << msg << '\n';
  getline(cin, input);
  
  while (input.empty() || (input[0] != 'Y' && input[0] != 'y' && input[0] != 'N' && input[0] != 'n')) {
    cout << "Invalid input!" << '\n';
    cout << msg << '\n';
    getline(cin, input);
  }
  
  return (input[0] == 'Y' || input[0] == 'y');
}

bool IO::inputSrc(char const *argv[]){
  string imagePath;
  
  cout << "Input absolute path to image file: ";
  getline(cin, imagePath);
  
  while (imagePath.empty() || imagePath.find_first_not_of(" \t\n\r") == string::npos) {
    cout << "Error: Empty path is not allowed.\n";
    cout << "Input absolute path to image file: ";
    getline(cin, imagePath);
  }
  
  imageSrcPath = fs::path(imagePath);

  while (!fs::exists(imageSrcPath)) {
    cout << "File not found: " << imageSrcPath << '\n';

    if (!validYN("Do you still want to input your picture? (Y/n)")) {
      return false;
    }

    cout << "Input absolute path to image file: ";
    getline(cin, imagePath);
    
    while (imagePath.empty() || imagePath.find_first_not_of(" \t\n\r") == string::npos) {
      cout << "Error: Empty path is not allowed.\n";
      cout << "Input absolute path to image file: ";
      getline(cin, imagePath);
    }
    
    imageSrcPath = fs::path(imagePath);
  }
  return true;
}

bool IO::inputThreshold() {
  string input;
  cout << "Input Variance Threshold: ";
  getline(cin, input);
  
  while (true) {
    try {
      if (!input.empty()) {
        VAR_THRESHOLD = stod(input);
        break;
      }
    } catch (...) {
      // Handle any exception from conversion
    }
    
    cout << "Invalid input! Please enter a valid number.\n";
    cout << "Input Variance Threshold: ";
    getline(cin, input);
  }

  return true;
}

bool IO::inputMinBlock(){
  string input;
  cout << "Input Minimum Block Size: ";
  getline(cin, input);
  
  while (true) {
    try {
      if (!input.empty()) {
        MIN_BLOCK_SIZE = stoi(input);
        break;
      }
    } catch (...) {
      // Handle any exception from conversion
    }
    
    cout << "Invalid input! Please enter a valid number.\n";
    cout << "Input Minimum Block Size: ";
    getline(cin, input);
  }

  return true;
}

bool IO::inputMethod(){
  string input;
  
  cout << "1. Variance" << '\n';
  cout << "2. Mean Absolute Deviation" << '\n';
  cout << "3. Maximum Difference" << '\n';
  cout << "4. Entropy" << '\n';
  cout << "5. Structural Similarity Index (SSIM) [BONUS]" << '\n';
  cout << "Choose method: ";
  getline(cin, input);
  
  while (true) {
    try {
      if (!input.empty()) {
        method = stoi(input);
        if (method >= 1 && method <= 5) {
          break;
        }
      }
    } catch (...) {
      // Handle any exception from conversion
    }
    
    cout << "Invalid input! Please enter a valid number between 1 and 5.\n";
    cout << "Choose method: ";
    getline(cin, input);
  }

  // if (method == 5) {
  //   cout << "Note: For SSIM, a higher threshold means better quality (e.g., 0.8-0.95).\n";
  //   cout << "      Unlike other methods where lower thresholds mean better quality.\n";
  // }

  return true;
}

bool IO::inputDest(char const *argv[]) {
  string outputPath;
  
  cout << "Enter absolute path for output image (including filename with extension): ";
  getline(cin, outputPath);
  
  while (outputPath.empty() || outputPath.find_first_not_of(" \t\n\r") == string::npos) {
    cout << "Error: Empty path is not allowed.\n";
    cout << "Enter absolute path for output image (including filename with extension): ";
    getline(cin, outputPath);
  }
  
  imageDestPath = fs::path(outputPath);

  fs::path parentPath = imageDestPath.parent_path();
  while (!fs::exists(parentPath)) {
    cout << "Directory does not exist: " << parentPath << '\n';
    
    if (validYN("Do you want to create this directory? (Y/n)")) {
      try {
        fs::create_directories(parentPath);
        if (!fs::exists(parentPath)) {
          cout << "Failed to create directory: " << parentPath << '\n';
        } else {
          break; 
        }
      } catch (const fs::filesystem_error& e) {
        cout << "Error creating directory: " << e.what() << '\n';
      }
    }
    
    cout << "Enter absolute path for output image (including filename with extension): ";
    getline(cin, outputPath);
    
    while (outputPath.empty() || outputPath.find_first_not_of(" \t\n\r") == string::npos) {
      cout << "Error: Empty path is not allowed.\n";
      cout << "Enter absolute path for output image (including filename with extension): ";
      getline(cin, outputPath);
    }
    
    imageDestPath = fs::path(outputPath);
    parentPath = imageDestPath.parent_path();
  }

  ofstream testFile(imageDestPath);
  while (!testFile) {
    cout << "Error: Cannot create file at " << imageDestPath << '\n';
    
    cout << "Enter absolute path for output image (including filename with extension): ";
    getline(cin, outputPath);
    
    while (outputPath.empty() || outputPath.find_first_not_of(" \t\n\r") == string::npos) {
      cout << "Error: Empty path is not allowed.\n";
      cout << "Enter absolute path for output image (including filename with extension): ";
      getline(cin, outputPath);
    }
    
    imageDestPath = fs::path(outputPath);
    
    parentPath = imageDestPath.parent_path();
    if (!fs::exists(parentPath)) {
      cout << "Directory does not exist: " << parentPath << '\n';
      
      if (validYN("Do you want to create this directory? (Y/n)")) {
        try {
          fs::create_directories(parentPath);
          if (!fs::exists(parentPath)) {
            cout << "Failed to create directory: " << parentPath << '\n';
            continue;
          }
        } catch (const fs::filesystem_error& e) {
          cout << "Error creating directory: " << e.what() << '\n';
          continue;
        }
      } else {
        continue; 
      }
    }
    
    testFile.open(imageDestPath);
  }
  testFile.close();

  cout << "Output file will be saved at: " << imageDestPath << '\n';
  return true;
}

bool IO::inputGifPath(char const *argv[]) {
  if (!validYN("Do you want to generate a GIF of the compression process? (Y/n)")) {
    GENERATE_GIF = false;
    return true;
  }

  GENERATE_GIF = true;

  string gifPathStr;
  cout << "Enter absolute path for output GIF (including filename with .gif extension): ";
  getline(cin, gifPathStr);
  
  while (gifPathStr.empty() || gifPathStr.find_first_not_of(" \t\n\r") == string::npos) {
    cout << "Error: Empty path is not allowed.\n";
    cout << "Enter absolute path for output GIF (including filename with .gif extension): ";
    getline(cin, gifPathStr);
  }
  
  gifPath = fs::path(gifPathStr);

  fs::path parentPath = gifPath.parent_path();
  while (!fs::exists(parentPath)) {
    cout << "Directory does not exist: " << parentPath << '\n';
    
    if (validYN("Do you want to create this directory? (Y/n)")) {
      try {
        fs::create_directories(parentPath);
        if (!fs::exists(parentPath)) {
          cout << "Failed to create directory: " << parentPath << '\n';
        } else {
          break; 
        }
      } catch (const fs::filesystem_error& e) {
        cout << "Error creating directory: " << e.what() << '\n';
      }
    }
    
    cout << "Enter absolute path for output GIF (including filename with .gif extension): ";
    getline(cin, gifPathStr);
    
    while (gifPathStr.empty() || gifPathStr.find_first_not_of(" \t\n\r") == string::npos) {
      cout << "Error: Empty path is not allowed.\n";
      cout << "Enter absolute path for output GIF (including filename with .gif extension): ";
      getline(cin, gifPathStr);
    }
    
    gifPath = fs::path(gifPathStr);
    parentPath = gifPath.parent_path();
  }

  ofstream testFile(gifPath);
  while (!testFile) {
    cout << "Error: Cannot create file at " << gifPath << '\n';
    
    cout << "Enter absolute path for output GIF (including filename with .gif extension): ";
    getline(cin, gifPathStr);
    
    while (gifPathStr.empty() || gifPathStr.find_first_not_of(" \t\n\r") == string::npos) {
      cout << "Error: Empty path is not allowed.\n";
      cout << "Enter absolute path for output GIF (including filename with .gif extension): ";
      getline(cin, gifPathStr);
    }
    
    gifPath = fs::path(gifPathStr);
    
    parentPath = gifPath.parent_path();
    if (!fs::exists(parentPath)) {
      cout << "Directory does not exist: " << parentPath << '\n';
      
      if (validYN("Do you want to create this directory? (Y/n)")) {
        try {
          fs::create_directories(parentPath);
          if (!fs::exists(parentPath)) {
            cout << "Failed to create directory: " << parentPath << '\n';
            continue;
          }
        } catch (const fs::filesystem_error& e) {
          cout << "Error creating directory: " << e.what() << '\n';
          continue;
        }
      } else {
        continue; 
      }
    }
    
    testFile.open(gifPath);
  }
  testFile.close();

  cout << "GIF file will be saved at: " << gifPath << '\n';
  return true;
}

void IO::renderQuadTreeAtDepth(QuadTreeNode* node, vector<uint8_t>& frameData, 
                              int imageWidth, int imageHeight, int depth) {
    if (node == nullptr) return;
    
    if (node->isLeafNode() || depth <= 0) {
        RGB mean = node->getMean();
        int x = node->getX();
        int y = node->getY();
        int width = node->getWidth();
        int height = node->getHeight();
        
        for (int i = y; i < min(y + height, imageHeight); ++i) {
            for (int j = x; j < min(x + width, imageWidth); ++j) {
                int idx = (i * imageWidth + j) * 3;
                frameData[idx] = mean.r;
                frameData[idx + 1] = mean.g;
                frameData[idx + 2] = mean.b;
            }
        }
    } else {
        for (int k = 0; k < 4; ++k) {
            renderQuadTreeAtDepth(node->getChildNode(k), frameData, imageWidth, imageHeight, depth - 1);
        }
    }
}

bool IO::createCompressionGif(const vector<vector<RGB>>& originalImage, const QuadTree& quadtree, int delayMs) {
    if (!GENERATE_GIF) {
        return true; 
    }
    
    if (originalImage.empty() || originalImage[0].empty()) {
        cerr << "Error: Invalid image dimensions for GIF creation" << endl;
        return false;
    }
    
    int height = originalImage.size();
    int width = originalImage[0].size();
    
    vector<vector<uint8_t>> frames;
    
    cout << "Creating GIF frames..." << endl;
    
    vector<uint8_t> originalFrame(width * height * 3);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int idx = (i * width + j) * 3;
            originalFrame[idx] = originalImage[i][j].r;
            originalFrame[idx + 1] = originalImage[i][j].g;
            originalFrame[idx + 2] = originalImage[i][j].b;
        }
    }
    frames.push_back(originalFrame);
    
    int maxDepth = quadtree.getMaxDepth();
    
    for (int depth = 1; depth <= maxDepth; depth++) {
        vector<uint8_t> frameData(width * height * 3, 0);
        renderQuadTreeAtDepth(quadtree.getRoot(), frameData, width, height, depth);
        frames.push_back(frameData);
    }
    
    cout << "Generating animated GIF with " << frames.size() << " frames..." << endl;
    bool success = createAnimatedGif(frames, width, height, 200, gifPath.string());
    
    if (success) {
        cout << "GIF successfully created at: " << gifPath << endl;
    } else {
        cerr << "Failed to create GIF" << endl;
    }
    
    return success;
}

bool IO::initInput(int argc, char const *argv[]) {
    if (!inputSrc(argv))                return false;
    if (!inputMethod())                 return false;
    if (!inputThreshold())              return false;
    if (!inputMinBlock())               return false;
    if (!inputDest(argv))               return false;
    if (!inputGifPath(argv))            return false;

    return true;
}