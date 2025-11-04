# OpenSSL Certificate Manager - Usage Examples

This document provides detailed examples of how to use the Certificate Manager application.

## Quick Start Guide

### 1. First Run - Initialize Directories

When you first run the application, initialize the PKI directory structure:

```
========================================
  OpenSSL Certificate Manager
========================================
1. Initialize PKI Directory Structure
2. Create Self-Signed Certificate
3. List Certificates
4. Display Certificate Info
5. Move Certificate
6. Exit
========================================
Enter your choice: 1
```

This creates the following directory structure:
```
pki/
├── own/
│   ├── certs/     (empty initially)
│   └── private/   (empty initially)
├── trusted/       (empty initially)
├── rejected/      (empty initially)
├── issuers/       (empty initially)
└── crl/           (empty initially)
```

### 2. Create Your First Certificate

Create a self-signed certificate for testing:

```
Enter your choice: 2

=== Create Self-Signed Certificate ===
Enter Common Name (e.g., example.com): localhost
Enter Organization (e.g., My Company): Development Team
Enter Country Code (e.g., US): US
Enter validity period in days (e.g., 365): 365
```

**Output:**
```
=== Creating Self-Signed Certificate ===
Common Name: localhost
Organization: Development Team
Country: US
Valid Days: 365

Generating RSA key pair (2048 bits)...
Key pair generated successfully!
Creating X509 certificate...
Certificate created successfully!

Saving certificate and private key...
  Certificate saved: pki/own/certs/localhost_cert.pem
  Private key saved: pki/own/private/localhost_key.pem

Certificate Information:
  Subject: /C=US/O=Development Team/CN=localhost
  Issuer: /C=US/O=Development Team/CN=localhost
  Serial Number: 1
  Valid From: Nov  4 21:00:00 2025 GMT
  Valid Until: Nov  4 21:00:00 2026 GMT

=== Certificate Creation Complete ===
```

### 3. List Certificates

View all certificates in a specific directory:

```
Enter your choice: 3

Select directory to list certificates from:
1. Own Certificates (own/certs)
2. Own Private Keys (own/private)
3. Trusted Certificates (trusted)
4. Rejected Certificates (rejected)
5. Issuer Certificates (issuers)
6. Certificate Revocation Lists (crl)
Enter choice: 1

Certificates in own/certs:
  1. localhost_cert.pem
```

### 4. Display Certificate Details

View detailed information about a certificate:

```
Enter your choice: 4

Select directory:
1. Own Certificates (own/certs)
2. Trusted Certificates (trusted)
3. Issuer Certificates (issuers)
Enter choice: 1

Available certificates:
  1. localhost_cert.pem
Enter certificate number to display: 1

Certificate loaded: pki/own/certs/localhost_cert.pem

=== Certificate Information ===
  Subject: /C=US/O=Development Team/CN=localhost
  Issuer: /C=US/O=Development Team/CN=localhost
  Serial Number: 1
  Valid From: Nov  4 21:00:00 2025 GMT
  Valid Until: Nov  4 21:00:00 2026 GMT
```

### 5. Move Certificate to Trusted Store

Move a certificate from "own" to "trusted" category:

```
Enter your choice: 5

Select source directory:
1. Own Certificates (own/certs)
2. Trusted Certificates (trusted)
3. Rejected Certificates (rejected)
4. Issuer Certificates (issuers)
Enter choice: 1

Available certificates:
  1. localhost_cert.pem
Enter certificate number to move: 1

Select destination directory:
1. Trusted Certificates (trusted)
2. Rejected Certificates (rejected)
3. Issuer Certificates (issuers)
Enter choice: 1

Certificate moved from own/certs to trusted
```

## Common Use Cases

### Use Case 1: Development Environment Setup

Create certificates for local development:

1. Initialize directories (option 1)
2. Create certificate for `localhost` (option 2)
3. Create certificate for `127.0.0.1` (option 2)
4. List your certificates to verify (option 3)

### Use Case 2: Certificate Authority Setup

Create and manage CA certificates:

1. Create root CA certificate with long validity:
   - Common Name: `Root CA`
   - Organization: `Your Organization`
   - Valid Days: `3650` (10 years)

2. Move to issuers directory (option 5)
   - Source: own/certs
   - Select the CA certificate
   - Destination: issuers

### Use Case 3: Trust Management

Organize certificates by trust level:

- **Trusted:** Certificates you explicitly trust
- **Rejected:** Certificates you explicitly don't trust
- **Own:** Certificates you created
- **Issuers:** Certificate Authority certificates

### Use Case 4: Web Server Certificate

Create a certificate for your web server:

```
Common Name: www.example.com
Organization: Example Corp
Country Code: US
Valid Days: 365
```

Files created:
- `pki/own/certs/www.example.com_cert.pem` - Certificate file
- `pki/own/private/www.example.com_key.pem` - Private key file

## Certificate File Format

All certificates are stored in PEM (Privacy Enhanced Mail) format:

**Certificate File Example (*.cert.pem):**
```
-----BEGIN CERTIFICATE-----
MIIDXTCCAkWgAwIBAgIBATANBgkqhkiG9w0BAQsFADBFMQswCQYDVQQGEwJVUzEZ
...
-----END CERTIFICATE-----
```

**Private Key File Example (*.key.pem):**
```
-----BEGIN PRIVATE KEY-----
MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQC7...
...
-----END PRIVATE KEY-----
```

## Best Practices

### Security
1. **Never share private keys** - Files in `pki/own/private/` should remain confidential
2. **Use strong passphrases** - Consider encrypting private keys (not implemented in this demo)
3. **Regular rotation** - Create new certificates before old ones expire
4. **Backup carefully** - If backing up, ensure private keys are encrypted

### Organization
1. **Use descriptive names** - Include domain/purpose in Common Name
2. **Document validity** - Keep track of expiration dates
3. **Separate by purpose** - Use the category directories appropriately
4. **Version control** - Only commit certificates, never private keys

### Development
1. **Short validity for testing** - Use 30-90 days for dev certificates
2. **Long validity for CA** - Root CA can have 10+ years
3. **Consistent naming** - Use conventions like `hostname_cert.pem` / `hostname_key.pem`

## Integrating with Applications

### Web Server (Apache)

```apache
SSLEngine on
SSLCertificateFile    /path/to/pki/own/certs/example.com_cert.pem
SSLCertificateKeyFile /path/to/pki/own/private/example.com_key.pem
```

### Web Server (Nginx)

```nginx
ssl_certificate     /path/to/pki/own/certs/example.com_cert.pem;
ssl_certificate_key /path/to/pki/own/private/example.com_key.pem;
```

### Node.js HTTPS Server

```javascript
const https = require('https');
const fs = require('fs');

const options = {
  cert: fs.readFileSync('pki/own/certs/localhost_cert.pem'),
  key: fs.readFileSync('pki/own/private/localhost_key.pem')
};

https.createServer(options, (req, res) => {
  res.writeHead(200);
  res.end('Hello secure world!');
}).listen(443);
```

### Python Flask

```python
from flask import Flask
app = Flask(__name__)

if __name__ == '__main__':
    app.run(
        ssl_context=(
            'pki/own/certs/localhost_cert.pem',
            'pki/own/private/localhost_key.pem'
        ),
        host='0.0.0.0',
        port=443
    )
```

## Advanced Features (Future Enhancements)

Potential additions to this application:

- **Certificate Signing Requests (CSR):** Generate CSRs for external CAs
- **Certificate Verification:** Validate certificate chains
- **CRL Management:** Create and manage Certificate Revocation Lists
- **OCSP Support:** Online Certificate Status Protocol
- **Key Encryption:** Password-protected private keys
- **Certificate Import/Export:** Support for PKCS#12 format
- **Automated Renewal:** Certificate expiration monitoring and renewal

## Troubleshooting

### Problem: Certificate Creation Fails

**Symptoms:** Error during certificate generation

**Solutions:**
- Ensure OpenSSL is properly installed
- Verify OPENSSL_ROOT_DIR environment variable
- Check write permissions on pki/ directory
- Ensure no special characters in input fields

### Problem: Cannot Find Certificate Files

**Symptoms:** Files not appearing in directory listings

**Solutions:**
- Verify pki/ directory exists
- Check file extensions (.pem)
- Ensure certificates were successfully saved
- Look for error messages during creation

### Problem: Certificate Shows as Invalid

**Symptoms:** Applications reject the certificate

**Solutions:**
- Check certificate dates (Valid From/Until)
- Verify Common Name matches your use case
- Remember: Self-signed certificates aren't trusted by default
- For browsers: Import to trusted certificate store manually

## Getting Help

For more information:
- See README.md for build and installation instructions
- Check OpenSSL documentation: https://www.openssl.org/docs/
- Review X.509 certificate standards: RFC 5280

## Example Session

Complete example of creating and managing a certificate:

```
OpenSSL Certificate Management System
Version 1.0
======================================

========================================
  OpenSSL Certificate Manager
========================================
1. Initialize PKI Directory Structure
2. Create Self-Signed Certificate
3. List Certificates
4. Display Certificate Info
5. Move Certificate
6. Exit
========================================
Enter your choice: 1

Initializing PKI directory structure...
  Created/Verified: pki/own/certs
  Created/Verified: pki/own/private
  Created/Verified: pki/trusted
  Created/Verified: pki/rejected
  Created/Verified: pki/issuers
  Created/Verified: pki/crl
PKI directory structure initialized successfully!

Enter your choice: 2

=== Create Self-Signed Certificate ===
Enter Common Name (e.g., example.com): test.local
Enter Organization (e.g., My Company): Test Organization
Enter Country Code (e.g., US): US
Enter validity period in days (e.g., 365): 90

=== Creating Self-Signed Certificate ===
...
Certificate created successfully!

Enter your choice: 3

Select directory to list certificates from:
1. Own Certificates (own/certs)
Enter choice: 1

Certificates in own/certs:
  1. test.local_cert.pem

Enter your choice: 6

Exiting Certificate Manager. Goodbye!
```
