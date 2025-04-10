#include "header/io.hpp"

bool IO::validYN(string msg){
  char confirm;
  cout << msg << '\n';
  cin >> confirm;
  while (confirm != 'Y' && confirm != 'y' && confirm != 'N' && confirm != 'n') {
    cout << "Invalid input!" << '\n';
    cout << msg << '\n';
    cin >> confirm;
  }
  if (confirm == 'Y' || confirm == 'y') return true;
  else return false;
}

bool IO::inputSrc(char const *argv[]){
  fs::path rootWorkspace = fs::canonical(fs::path(argv[0]).parent_path() / "..");
  rootWorkspace += "/test/";
    
  cout << "Input Image Name in test folder (with extension): ";
  string imageName;
  cin >> imageName;
  imageSrcPath = rootWorkspace / imageName;

  while (!fs::exists(imageSrcPath)) {
    cout << "File not found: " << imageSrcPath << '\n';

    if (!validYN("Do you still want to input your picture? (Y/n)")) {
      return false;
    }

    cout << "Input Image Name in test folder (with extension): ";
    cin >> imageName;
    imageSrcPath = rootWorkspace / imageName;
  }
  return true;
}

bool IO::inputThreshold() {
  cout << "Input Variance Threshold: ";
  
  while (!(cin >> VAR_THRESHOLD)) {
    cout << "Invalid input! Please enter a valid number.\n";
    cout << "Input Variance Threshold: ";
    
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }

  return true;
}


bool IO::inputMinBlock(){
  cout << "Input Minimum Block Size: ";
  
  while (!(cin >> MIN_BLOCK_SIZE)) {
    cout << "Invalid input! Please enter a valid number.\n";
    cout << "Input Minimum Block Size: ";
    
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }

  return true;
}

bool IO::inputMethod(){
  cout << "1. Variance" << '\n';
  cout << "2. Mean Absolute Deviation" << '\n';
  cout << "3. Maximum Difference" << '\n';
  cout << "4. Entropy" << '\n';
  cout << "5. Structural Similarity Index (SSIM) [BONUS]" << '\n';
  cout << "Choose method: ";

  while (!(cin >> method) || method < 1 || method > 5) {
    cout << "Invalid input! Please enter a valid number between 1 and 5.\n";
    cout << "Choose method: ";
    
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }

  if (method == 5) {
    cout << "Note: For SSIM, a higher threshold means better quality (e.g., 0.8-0.95).\n";
    cout << "      Unlike other methods where lower thresholds mean better quality.\n";
  }

  return true;
}

bool IO::inputDest(char const *argv[]) {
  fs::path rootWorkspace = fs::canonical(fs::path(argv[0]).parent_path() / "..");
  rootWorkspace += "/test/";
  string extension = imageSrcPath.extension().string();

  string imageName;
  cout << "Enter output image name (without extension, e.g., result): ";
  cin >> imageName;

  imageDestPath = rootWorkspace / (imageName + extension);

  if (!fs::exists(rootWorkspace)) {
    fs::create_directories(rootWorkspace);
  }

  ofstream testFile(imageDestPath);
  while (!testFile) {
    cout << "Error: Cannot create file at " << imageDestPath << '\n';
    cout << "Enter output image name (without extension, e.g., result): ";
    cin >> imageName;

    imageDestPath = rootWorkspace / (imageName + extension);
    return false;
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
  fs::path rootWorkspace = fs::canonical(fs::path(argv[0]).parent_path() / "..");
  rootWorkspace += "/test/";

  string gifName;
  cout << "Enter output GIF name (without extension, e.g., compression_process): ";
  cin >> gifName;

  gifPath = rootWorkspace / (gifName + ".gif");

  if (!fs::exists(rootWorkspace)) {
    fs::create_directories(rootWorkspace);
  }

  ofstream testFile(gifPath);
  if (!testFile) {
    cout << "Error: Cannot create file at " << gifPath << '\n';
    return false;
  }
  testFile.close();

  cout << "GIF file will be saved at: " << gifPath << '\n';
  return true;
}

bool IO::initInput(int argc, char const *argv[]){
  if (!inputSrc(argv))                return false;
  if (!inputMethod())                 return false;
  if (!inputThreshold())              return false;
  if (!inputMinBlock())               return false;
  if (!inputDest(argv))               return false;
  // if (!inputGifPath(argv))            return false;

  return true;
}