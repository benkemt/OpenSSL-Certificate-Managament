#include "CertificateManager.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <openssl/rsa.h>
#include <openssl/x509v3.h>
#include <openssl/bn.h>
#include <openssl/asn1.h>

namespace fs = std::filesystem;

CertificateManager::CertificateManager(const std::string& pkiBasePath)
    : pkiBasePath_(pkiBasePath) {
}

CertificateManager::~CertificateManager() {
}

bool CertificateManager::initializeDirectories() {
    std::vector<std::string> directories = {
        getOwnCertsPath(),
        getOwnPrivatePath(),
        getTrustedPath(),
        getRejectedPath(),
        getIssuersPath(),
        getCRLPath()
    };

    std::cout << "Initializing PKI directory structure..." << std::endl;
    
    for (const auto& dir : directories) {
        if (!createDirectory(dir)) {
            std::cerr << "Failed to create directory: " << dir << std::endl;
            return false;
        }
        std::cout << "  Created/Verified: " << dir << std::endl;
    }
    
    std::cout << "PKI directory structure initialized successfully!" << std::endl;
    return true;
}

bool CertificateManager::createSelfSignedCertificate(
    const std::string& commonName,
    const std::string& organization,
    const std::string& country,
    int validDays) {
    
    std::cout << "\n=== Creating Self-Signed Certificate ===" << std::endl;
    std::cout << "Common Name: " << commonName << std::endl;
    std::cout << "Organization: " << organization << std::endl;
    std::cout << "Country: " << country << std::endl;
    std::cout << "Valid Days: " << validDays << std::endl;

    // Generate RSA key pair
    std::cout << "\nGenerating RSA key pair (2048 bits)..." << std::endl;
    EVP_PKEY* pkey = EVP_PKEY_new();
    if (!pkey) {
        std::cerr << "Failed to create EVP_PKEY" << std::endl;
        return false;
    }

    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
    if (!ctx) {
        EVP_PKEY_free(pkey);
        std::cerr << "Failed to create EVP_PKEY_CTX" << std::endl;
        return false;
    }

    if (EVP_PKEY_keygen_init(ctx) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(pkey);
        std::cerr << "Failed to initialize key generation" << std::endl;
        return false;
    }

    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(pkey);
        std::cerr << "Failed to set key size" << std::endl;
        return false;
    }

    if (EVP_PKEY_keygen(ctx, &pkey) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(pkey);
        std::cerr << "Failed to generate key" << std::endl;
        return false;
    }

    EVP_PKEY_CTX_free(ctx);
    std::cout << "Key pair generated successfully!" << std::endl;

    // Create X509 certificate
    std::cout << "Creating X509 certificate..." << std::endl;
    X509* cert = X509_new();
    if (!cert) {
        EVP_PKEY_free(pkey);
        std::cerr << "Failed to create X509 certificate" << std::endl;
        return false;
    }

    // Set version to X509 v3
    X509_set_version(cert, 2);

    // Set serial number
    ASN1_INTEGER_set(X509_get_serialNumber(cert), 1);

    // Set validity period
    X509_gmtime_adj(X509_get_notBefore(cert), 0);
    X509_gmtime_adj(X509_get_notAfter(cert), validDays * 24 * 3600);

    // Set public key
    X509_set_pubkey(cert, pkey);

    // Set subject name
    X509_NAME* name = X509_get_subject_name(cert);
    X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC,
        (unsigned char*)country.c_str(), -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC,
        (unsigned char*)organization.c_str(), -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC,
        (unsigned char*)commonName.c_str(), -1, -1, 0);

    // Self-signed: issuer = subject
    X509_set_issuer_name(cert, name);

    // Sign the certificate
    if (!X509_sign(cert, pkey, EVP_sha256())) {
        X509_free(cert);
        EVP_PKEY_free(pkey);
        std::cerr << "Failed to sign certificate" << std::endl;
        return false;
    }

    std::cout << "Certificate created successfully!" << std::endl;

    // Save certificate and private key
    std::string certFilename = commonName + "_cert.pem";
    std::string keyFilename = commonName + "_key.pem";

    std::cout << "\nSaving certificate and private key..." << std::endl;
    
    if (!saveCertificate(cert, certFilename)) {
        X509_free(cert);
        EVP_PKEY_free(pkey);
        return false;
    }

    if (!savePrivateKey(pkey, keyFilename)) {
        X509_free(cert);
        EVP_PKEY_free(pkey);
        return false;
    }

    std::cout << "\nCertificate Information:" << std::endl;
    displayCertificateInfo(cert);

    // Clean up
    X509_free(cert);
    EVP_PKEY_free(pkey);

    std::cout << "\n=== Certificate Creation Complete ===" << std::endl;
    return true;
}

bool CertificateManager::saveCertificate(X509* cert, const std::string& filename, const std::string& subdir) {
    std::string fullPath = getFullPath(subdir) + "/" + filename;
    
    FILE* fp = fopen(fullPath.c_str(), "wb");
    if (!fp) {
        std::cerr << "Failed to open file for writing: " << fullPath << std::endl;
        return false;
    }

    if (!PEM_write_X509(fp, cert)) {
        fclose(fp);
        std::cerr << "Failed to write certificate to file" << std::endl;
        return false;
    }

    fclose(fp);
    std::cout << "  Certificate saved: " << fullPath << std::endl;
    return true;
}

bool CertificateManager::savePrivateKey(EVP_PKEY* pkey, const std::string& filename, const std::string& subdir) {
    std::string fullPath = getFullPath(subdir) + "/" + filename;
    
    FILE* fp = fopen(fullPath.c_str(), "wb");
    if (!fp) {
        std::cerr << "Failed to open file for writing: " << fullPath << std::endl;
        return false;
    }

    if (!PEM_write_PrivateKey(fp, pkey, nullptr, nullptr, 0, nullptr, nullptr)) {
        fclose(fp);
        std::cerr << "Failed to write private key to file" << std::endl;
        return false;
    }

    fclose(fp);
    std::cout << "  Private key saved: " << fullPath << std::endl;
    return true;
}

X509* CertificateManager::loadCertificate(const std::string& filename, const std::string& subdir) {
    std::string fullPath = getFullPath(subdir) + "/" + filename;
    
    FILE* fp = fopen(fullPath.c_str(), "rb");
    if (!fp) {
        std::cerr << "Failed to open file for reading: " << fullPath << std::endl;
        return nullptr;
    }

    X509* cert = PEM_read_X509(fp, nullptr, nullptr, nullptr);
    fclose(fp);

    if (!cert) {
        std::cerr << "Failed to read certificate from file" << std::endl;
        return nullptr;
    }

    std::cout << "Certificate loaded: " << fullPath << std::endl;
    return cert;
}

EVP_PKEY* CertificateManager::loadPrivateKey(const std::string& filename, const std::string& subdir) {
    std::string fullPath = getFullPath(subdir) + "/" + filename;
    
    FILE* fp = fopen(fullPath.c_str(), "rb");
    if (!fp) {
        std::cerr << "Failed to open file for reading: " << fullPath << std::endl;
        return nullptr;
    }

    EVP_PKEY* pkey = PEM_read_PrivateKey(fp, nullptr, nullptr, nullptr);
    fclose(fp);

    if (!pkey) {
        std::cerr << "Failed to read private key from file" << std::endl;
        return nullptr;
    }

    std::cout << "Private key loaded: " << fullPath << std::endl;
    return pkey;
}

bool CertificateManager::moveCertificate(const std::string& filename, 
                                         const std::string& fromSubdir, 
                                         const std::string& toSubdir) {
    std::string fromPath = getFullPath(fromSubdir) + "/" + filename;
    std::string toPath = getFullPath(toSubdir) + "/" + filename;

    try {
        fs::rename(fromPath, toPath);
        std::cout << "Certificate moved from " << fromSubdir << " to " << toSubdir << std::endl;
        return true;
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Failed to move certificate: " << e.what() << std::endl;
        return false;
    }
}

std::vector<std::string> CertificateManager::listCertificates(const std::string& subdir) {
    std::vector<std::string> certificates;
    std::string dirPath = getFullPath(subdir);

    try {
        if (fs::exists(dirPath) && fs::is_directory(dirPath)) {
            for (const auto& entry : fs::directory_iterator(dirPath)) {
                if (entry.is_regular_file()) {
                    std::string filename = entry.path().filename().string();
                    if (filename.size() > 4 && filename.substr(filename.size() - 4) == ".pem") {
                        certificates.push_back(filename);
                    }
                }
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Error listing certificates: " << e.what() << std::endl;
    }

    return certificates;
}

void CertificateManager::displayCertificateInfo(X509* cert) {
    if (!cert) {
        std::cerr << "No certificate to display" << std::endl;
        return;
    }

    // Subject
    char* subject = X509_NAME_oneline(X509_get_subject_name(cert), nullptr, 0);
    std::cout << "  Subject: " << (subject ? subject : "N/A") << std::endl;
    OPENSSL_free(subject);

    // Issuer
    char* issuer = X509_NAME_oneline(X509_get_issuer_name(cert), nullptr, 0);
    std::cout << "  Issuer: " << (issuer ? issuer : "N/A") << std::endl;
    OPENSSL_free(issuer);

    // Serial number
    ASN1_INTEGER* serial = X509_get_serialNumber(cert);
    if (serial) {
        BIGNUM* bn = ASN1_INTEGER_to_BN(serial, nullptr);
        if (bn) {
            char* serialStr = BN_bn2dec(bn);
            std::cout << "  Serial Number: " << (serialStr ? serialStr : "N/A") << std::endl;
            OPENSSL_free(serialStr);
            BN_free(bn);
        }
    }

    // Validity
    BIO* bio = BIO_new(BIO_s_mem());
    if (bio) {
        ASN1_TIME_print(bio, X509_get_notBefore(cert));
        char buffer[256];
        int len = BIO_read(bio, buffer, sizeof(buffer) - 1);
        if (len > 0) {
            buffer[len] = '\0';
            std::cout << "  Valid From: " << buffer << std::endl;
        }
        BIO_free(bio);
    }

    bio = BIO_new(BIO_s_mem());
    if (bio) {
        ASN1_TIME_print(bio, X509_get_notAfter(cert));
        char buffer[256];
        int len = BIO_read(bio, buffer, sizeof(buffer) - 1);
        if (len > 0) {
            buffer[len] = '\0';
            std::cout << "  Valid Until: " << buffer << std::endl;
        }
        BIO_free(bio);
    }
}

std::string CertificateManager::getFullPath(const std::string& subdir) const {
    return pkiBasePath_ + "/" + subdir;
}

bool CertificateManager::createDirectory(const std::string& path) {
    try {
        if (!fs::exists(path)) {
            fs::create_directories(path);
        }
        return true;
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Failed to create directory " << path << ": " << e.what() << std::endl;
        return false;
    }
}

bool CertificateManager::directoryExists(const std::string& path) {
    return fs::exists(path) && fs::is_directory(path);
}
