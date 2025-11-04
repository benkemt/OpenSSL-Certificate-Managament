#pragma once

#include <string>
#include <vector>
#include <memory>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/evp.h>

// PKI Directory Structure:
// pki/
// ├── own/
// │   ├── certs/     (own certificates)
// │   └── private/   (private keys)
// ├── trusted/       (trusted certificates)
// ├── rejected/      (rejected certificates)
// ├── issuers/       (issuer certificates)
// └── crl/           (certificate revocation lists)

class CertificateManager {
public:
    CertificateManager(const std::string& pkiBasePath = "pki");
    ~CertificateManager();

    // Initialize PKI directory structure
    bool initializeDirectories();

    // Certificate creation
    bool createSelfSignedCertificate(
        const std::string& commonName,
        const std::string& organization,
        const std::string& country,
        int validDays = 365
    );

    // Certificate management
    bool saveCertificate(X509* cert, const std::string& filename, const std::string& subdir = "own/certs");
    bool savePrivateKey(EVP_PKEY* pkey, const std::string& filename, const std::string& subdir = "own/private");
    
    X509* loadCertificate(const std::string& filename, const std::string& subdir = "own/certs");
    EVP_PKEY* loadPrivateKey(const std::string& filename, const std::string& subdir = "own/private");

    // Certificate operations
    bool moveCertificate(const std::string& filename, const std::string& fromSubdir, const std::string& toSubdir);
    std::vector<std::string> listCertificates(const std::string& subdir = "own/certs");
    
    // Display certificate information
    void displayCertificateInfo(X509* cert);

    // Get PKI paths
    std::string getOwnCertsPath() const { return pkiBasePath_ + "/own/certs"; }
    std::string getOwnPrivatePath() const { return pkiBasePath_ + "/own/private"; }
    std::string getTrustedPath() const { return pkiBasePath_ + "/trusted"; }
    std::string getRejectedPath() const { return pkiBasePath_ + "/rejected"; }
    std::string getIssuersPath() const { return pkiBasePath_ + "/issuers"; }
    std::string getCRLPath() const { return pkiBasePath_ + "/crl"; }

private:
    std::string pkiBasePath_;
    
    // Helper methods
    std::string getFullPath(const std::string& subdir) const;
    bool createDirectory(const std::string& path);
    bool directoryExists(const std::string& path);
};
