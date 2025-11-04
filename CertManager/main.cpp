#include "CertificateManager.h"
#include <iostream>
#include <string>
#include <limits>

void displayMenu() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "  OpenSSL Certificate Manager" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. Initialize PKI Directory Structure" << std::endl;
    std::cout << "2. Create Self-Signed Certificate" << std::endl;
    std::cout << "3. List Certificates" << std::endl;
    std::cout << "4. Display Certificate Info" << std::endl;
    std::cout << "5. Move Certificate" << std::endl;
    std::cout << "6. Exit" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Enter your choice: ";
}

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void listCertificatesMenu(CertificateManager& manager) {
    std::cout << "\nSelect directory to list certificates from:" << std::endl;
    std::cout << "1. Own Certificates (own/certs)" << std::endl;
    std::cout << "2. Own Private Keys (own/private)" << std::endl;
    std::cout << "3. Trusted Certificates (trusted)" << std::endl;
    std::cout << "4. Rejected Certificates (rejected)" << std::endl;
    std::cout << "5. Issuer Certificates (issuers)" << std::endl;
    std::cout << "6. Certificate Revocation Lists (crl)" << std::endl;
    std::cout << "Enter choice: ";
    
    int choice;
    std::cin >> choice;
    clearInputBuffer();

    std::string subdir;
    switch (choice) {
        case 1: subdir = "own/certs"; break;
        case 2: subdir = "own/private"; break;
        case 3: subdir = "trusted"; break;
        case 4: subdir = "rejected"; break;
        case 5: subdir = "issuers"; break;
        case 6: subdir = "crl"; break;
        default:
            std::cout << "Invalid choice!" << std::endl;
            return;
    }

    std::cout << "\nCertificates in " << subdir << ":" << std::endl;
    auto certs = manager.listCertificates(subdir);
    
    if (certs.empty()) {
        std::cout << "  No certificates found." << std::endl;
    }
    else {
        for (size_t i = 0; i < certs.size(); ++i) {
            std::cout << "  " << (i + 1) << ". " << certs[i] << std::endl;
        }
    }
}

void displayCertificateInfoMenu(CertificateManager& manager) {
    std::cout << "\nSelect directory:" << std::endl;
    std::cout << "1. Own Certificates (own/certs)" << std::endl;
    std::cout << "2. Trusted Certificates (trusted)" << std::endl;
    std::cout << "3. Issuer Certificates (issuers)" << std::endl;
    std::cout << "Enter choice: ";
    
    int choice;
    std::cin >> choice;
    clearInputBuffer();

    std::string subdir;
    switch (choice) {
        case 1: subdir = "own/certs"; break;
        case 2: subdir = "trusted"; break;
        case 3: subdir = "issuers"; break;
        default:
            std::cout << "Invalid choice!" << std::endl;
            return;
    }

    auto certs = manager.listCertificates(subdir);
    if (certs.empty()) {
        std::cout << "No certificates found in " << subdir << std::endl;
        return;
    }

    std::cout << "\nAvailable certificates:" << std::endl;
    for (size_t i = 0; i < certs.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << certs[i] << std::endl;
    }

    std::cout << "Enter certificate number to display: ";
    int certChoice;
    std::cin >> certChoice;
    clearInputBuffer();

    if (certChoice < 1 || certChoice > static_cast<int>(certs.size())) {
        std::cout << "Invalid certificate number!" << std::endl;
        return;
    }

    X509* cert = manager.loadCertificate(certs[certChoice - 1], subdir);
    if (cert) {
        std::cout << "\n=== Certificate Information ===" << std::endl;
        manager.displayCertificateInfo(cert);
        X509_free(cert);
    }
}

void moveCertificateMenu(CertificateManager& manager) {
    std::cout << "\nSelect source directory:" << std::endl;
    std::cout << "1. Own Certificates (own/certs)" << std::endl;
    std::cout << "2. Trusted Certificates (trusted)" << std::endl;
    std::cout << "3. Rejected Certificates (rejected)" << std::endl;
    std::cout << "4. Issuer Certificates (issuers)" << std::endl;
    std::cout << "Enter choice: ";
    
    int fromChoice;
    std::cin >> fromChoice;
    clearInputBuffer();

    std::string fromSubdir;
    switch (fromChoice) {
        case 1: fromSubdir = "own/certs"; break;
        case 2: fromSubdir = "trusted"; break;
        case 3: fromSubdir = "rejected"; break;
        case 4: fromSubdir = "issuers"; break;
        default:
            std::cout << "Invalid choice!" << std::endl;
            return;
    }

    auto certs = manager.listCertificates(fromSubdir);
    if (certs.empty()) {
        std::cout << "No certificates found in " << fromSubdir << std::endl;
        return;
    }

    std::cout << "\nAvailable certificates:" << std::endl;
    for (size_t i = 0; i < certs.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << certs[i] << std::endl;
    }

    std::cout << "Enter certificate number to move: ";
    int certChoice;
    std::cin >> certChoice;
    clearInputBuffer();

    if (certChoice < 1 || certChoice > static_cast<int>(certs.size())) {
        std::cout << "Invalid certificate number!" << std::endl;
        return;
    }

    std::cout << "\nSelect destination directory:" << std::endl;
    std::cout << "1. Trusted Certificates (trusted)" << std::endl;
    std::cout << "2. Rejected Certificates (rejected)" << std::endl;
    std::cout << "3. Issuer Certificates (issuers)" << std::endl;
    std::cout << "Enter choice: ";
    
    int toChoice;
    std::cin >> toChoice;
    clearInputBuffer();

    std::string toSubdir;
    switch (toChoice) {
        case 1: toSubdir = "trusted"; break;
        case 2: toSubdir = "rejected"; break;
        case 3: toSubdir = "issuers"; break;
        default:
            std::cout << "Invalid choice!" << std::endl;
            return;
    }

    manager.moveCertificate(certs[certChoice - 1], fromSubdir, toSubdir);
}

int main() {
    std::cout << "OpenSSL Certificate Management System" << std::endl;
    std::cout << "Version 1.0" << std::endl;
    std::cout << "======================================\n" << std::endl;

    CertificateManager manager("pki");

    bool running = true;
    while (running) {
        displayMenu();
        
        int choice;
        std::cin >> choice;
        clearInputBuffer();

        switch (choice) {
            case 1: {
                manager.initializeDirectories();
                break;
            }
            case 2: {
                std::string commonName, organization, country;
                int validDays;

                std::cout << "\n=== Create Self-Signed Certificate ===" << std::endl;
                std::cout << "Enter Common Name (e.g., example.com): ";
                std::getline(std::cin, commonName);
                
                std::cout << "Enter Organization (e.g., My Company): ";
                std::getline(std::cin, organization);
                
                std::cout << "Enter Country Code (e.g., US): ";
                std::getline(std::cin, country);
                
                std::cout << "Enter validity period in days (e.g., 365): ";
                std::cin >> validDays;
                clearInputBuffer();

                manager.createSelfSignedCertificate(commonName, organization, country, validDays);
                break;
            }
            case 3: {
                listCertificatesMenu(manager);
                break;
            }
            case 4: {
                displayCertificateInfoMenu(manager);
                break;
            }
            case 5: {
                moveCertificateMenu(manager);
                break;
            }
            case 6: {
                std::cout << "\nExiting Certificate Manager. Goodbye!" << std::endl;
                running = false;
                break;
            }
            default: {
                std::cout << "\nInvalid choice! Please try again." << std::endl;
                break;
            }
        }
    }

    return 0;
}
