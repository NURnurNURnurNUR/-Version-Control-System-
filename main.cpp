#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <openssl/sha.h>
#include <sstream>
#include <chrono>
#include <iomanip>

namespace fs = std::filesystem;

class VCS_Repo {

private:
	fs::path RepoPath = fs::current_path();
    
	std::string calculateFileHash(const fs::path& filePath) {
		std::ifstream fileStream(filePath, std::ios::binary);
		std::vector<unsigned char> fileBuffer(std::istreambuf_iterator<char>(fileStream), {});

		unsigned char hash[SHA256_DIGEST_LENGTH];
		SHA256(fileBuffer.data(), fileBuffer.size(), hash);

		std::ostringstream hashStream;
		for (unsigned char c : hash) {
			hashStream << std::hex << static_cast<int>(c);
		}
		return hashStream.str();
	}

    std::string calculateHash(const std::string& input) {
        unsigned char buffer[SHA256_DIGEST_LENGTH];
        const unsigned char* data = reinterpret_cast<const unsigned char*>(input.c_str());
        SHA256(data, input.length(), buffer);

        std::stringstream ss;
        for(unsigned char s : buffer) {
            ss << std::hex << static_cast<int>(s);
        }
        return ss.str();
    }
    

    std::string generateCommitDirectoryName(){
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::ostringstream oss;
        oss<<std::put_time(std::localtime(&now_c), "%Y%m%d_%H%M%S");
        return oss.str();
    }

public:
	
	bool SearchLine(std::ifstream file, std::string& TargetLine) {
		return true;
	}


	void TrackFile(const std::string& filename) {
		fs::path trackFilepath = (RepoPath / ".AKNUR" / "tracked_files.txt");

		std::ifstream trackfileIn(trackFilepath);
		std::string existingFilename = filename;
		bool isAlreadyTracked = false;
		std::string line;
		if (trackfileIn.is_open()) {
			trackfileIn.seekg(0, trackfileIn.beg);
			while (std::getline(trackfileIn, line)) {
						if (line == existingFilename) {
							isAlreadyTracked = true;
							break;
						}
					}
					trackfileIn.close();
		}
    
		
		if (isAlreadyTracked == false) {
			std::ofstream trackfileOut(trackFilepath, std::ios::app);
			if (!trackfileOut) {
				std::cerr << "Error in opening tracked files list for writtining " << std::endl;
				return;
			}
			trackfileOut << filename << std::endl;
			std::cout << "File tracked: " << filename << std::endl;
			trackfileOut.close();
		}
		else {
			std::cout << "File already tracked: " << filename << std::endl;
		}
	}
		
	

	void initialize() {
		fs::create_directory(RepoPath / ".AKNUR");
		fs::create_directory(RepoPath / ".AKNUR/commits");
	
	}
	void CommitFile() {
        std::string commitDirName = generateCommitDirectoryName();
        std::string currentHash = calculateHash(generateCommitDirectoryName());
        fs:: path commitPath = RepoPath/ ".AKNUR/commits"/ currentHash; 
        fs::create_directories(commitPath);

        std::ofstream logFile(RepoPath/ ".AKNUR"/ "commit_log.txt", std::ios::app);
        logFile << "Commit: " << currentHash << std::endl;


		std::ifstream trackedFiles(RepoPath / ".AKNUR" / "tracked_files.txt");
		std::string filename;

		while (std::getline(trackedFiles, filename)) {
            if (filename.empty()) continue;

			fs::path filePath = RepoPath / filename;
            if(!fs::exists(filePath)){
                std::cerr << "File does not exist: "<<filename<<std::endl;
                continue;
            }

			
            fs::copy(filePath, commitPath/ filename, fs::copy_options::overwrite_existing);
            logFile<<"  - "<<filename<<" committed."<<std::endl;
        }
        logFile<<std::endl;
    }

    void CompareWithCommit(const std::string& commitHash){
        fs::path commitPath = RepoPath/ ".AKNUR/commits"/ commitHash;

        if (!fs::exists(commitPath)){
            std::cerr << "Commit was not found: " << commitHash <<std::endl;
            return;
        }

        std::ifstream trackedFiles(RepoPath/ ".AKNUR"/ "tracked_files.txt");
        std::string filename;

        while (std::getline(trackedFiles, filename)){
            if (filename.empty()) continue;
            fs::path currentFilePath = RepoPath/filename;
            fs::path commitFilePath = commitPath/ filename;

            if(!fs::exists(currentFilePath)){
                std::cout << "File is missing in current workspace: " << filename <<std::endl;
                continue;
            }
            if (!fs::exists(commitFilePath)){
                std::cout << "File is missing in commit " << filename << std::endl;
                continue;
            }
            std::string currentHash = calculateFileHash(currentFilePath);
            std::string commitHash = calculateFileHash(commitFilePath);

            if(currentHash != commitHash){
                std::cout << "File content changed: "<< filename << std::endl;
            } else {
                std::cout << "No content change detected: " << filename << std::endl;

            }
        }

    }


    void ViewCommitLog(){
        fs::path logFilePath = RepoPath/ ".AKNUR"/ "commit_log.txt";

        if(!fs::exists(logFilePath)){
            std::cout << "No commit log found " << std::endl;
            return;
        }

        std::ifstream logFile(logFilePath);
        std::string line;
        while (std::getline(logFile, line)){
            std::cout << line << std::endl;
        }
    }

    void ReverseCommit(const std::string& commitHash){
        fs::path commitPath = RepoPath/ ".AKNUR/commits"/ commitHash;

        if(!fs::exists(commitPath)){
            std::cerr << "Commit was not found. "<< commitHash << std::endl;
            return;
        }
        for(const auto& file : fs::directory_iterator(commitPath)){
            if(!file.is_directory()){
               fs::path filePath = RepoPath/ file.path().filename(); 
            
            fs::create_directories(filePath.parent_path());

            try{
                fs::copy(file.path(), filePath, fs::copy_options::overwrite_existing);
                std::cout << "Reverted file: " << filePath << std::endl;
                
            } catch(const std::exception& e){
                std::cerr << "Error reverting file" << filePath << ": " << e.what() << std::endl;
            }            
            
        }
        }        
        std::cout << "Reverted to commit: " << commitHash << std::endl;
    }
    
    void CheckIntegrity() {
        std::ifstream trackedFiles(RepoPath / ".AKNUR" / "tracked_files.txt");
        std::string filename;
        
        
         while (std::getline(trackedFiles, filename)) {
            if (filename.empty()) continue;

        
        fs::path filePath = RepoPath / filename;
        fs::path commitPath = RepoPath / ".AKNUR/commits" / calculateFileHash(filePath) / filename;
        
        if (!fs::exists(commitPath)) {
            std::cout << "No commit found for file: " << filename << std::endl;
            continue;
        }

        std::string currentFileHash = calculateFileHash(filePath);
        std::string committedFileHash = calculateFileHash(commitPath);

        
        if (currentFileHash != committedFileHash) {
            std::cerr << "Integrity check failed for file: " << filename << std::endl;
        } else {
            std::cout << "Integrity check passed for file: " << filename << std::endl;
        }
    }
}


    void DisplayHelp() {
        std::cout << "VCS_Repo Commands:\n";
        std::cout << "  init              - Initialize the repository\n";
        std::cout << "  track             - Track a file. Usage: track [filename]\n";
        std::cout << "  commit            - Commit changes\n";
        std::cout << "  log               - View commit log\n";
        std::cout << "  compare           - Compare with a commit. Usage: compare [commitHash]\n";
        std::cout << "  reverse           - Reverse to a commit. Usage: reverse [commitHash]\n";
        std::cout << "  check-integrity   - Perform integrity check on committed files\n";
        std::cout << "  help              - Display this help information\n";
    }
};

	int main(int argc, char* argv[]) {
		if (argc == 1) {
			std::cout << "No command provided. " << std::endl;
			return 1;
		}

		VCS_Repo VCS;

		if (std::strcmp(argv[1], "init") == 0) {
			VCS.initialize();
		}
		else if (std::strcmp(argv[1], "track") == 0 && argc > 2) {
			VCS.TrackFile(argv[2]);
		}
		else if (std::strcmp(argv[1], "commit") == 0) {
			VCS.CommitFile();
		}
        else if (std::strcmp(argv[1], "compare") == 0 && argc > 2) {
                VCS.CompareWithCommit(argv[2]);
        }
        else if(std::strcmp(argv[1], "log") == 0) {
            VCS.ViewCommitLog();
        }
        else if (std::strcmp(argv[1], "reverse") == 0 && argc > 2) {
            VCS.ReverseCommit(argv[2]);
        }
        else if (std::strcmp(argv[1], "check-integrity") == 0) {
             VCS.CheckIntegrity();
        }
        else if (std::strcmp(argv[1], "help") == 0) {
            VCS.DisplayHelp();
        }
	    else {
			std::cerr << "Invalid command or insufficient arguments " << std::endl;
            std::cerr << "Use help to see the list of commands " << std::endl;
		}

		return 0;
    }