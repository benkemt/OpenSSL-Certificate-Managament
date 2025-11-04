# OpenSSL Certificate Manager - Quick Reference

## Command Line Quick Reference

### OpenSSL Commands (for reference)

If you need to work with the generated certificates using OpenSSL command line:

#### View Certificate Details
```bash
openssl x509 -in pki/own/certs/example_cert.pem -text -noout
```

#### Verify Certificate
```bash
openssl verify -CAfile pki/issuers/ca_cert.pem pki/own/certs/example_cert.pem
```

#### View Certificate Dates
```bash
openssl x509 -in pki/own/certs/example_cert.pem -noout -dates
```

#### View Certificate Subject
```bash
openssl x509 -in pki/own/certs/example_cert.pem -noout -subject
```

#### View Certificate Issuer
```bash
openssl x509 -in pki/own/certs/example_cert.pem -noout -issuer
```

#### Check Private Key
```bash
openssl rsa -in pki/own/private/example_key.pem -check -noout
```

#### Extract Public Key from Certificate
```bash
openssl x509 -in pki/own/certs/example_cert.pem -pubkey -noout > public_key.pem
```

#### Convert PEM to DER format
```bash
openssl x509 -in pki/own/certs/example_cert.pem -outform DER -out example_cert.der
```

#### Convert to PKCS#12 (for Windows Certificate Store)
```bash
openssl pkcs12 -export -out example.pfx -inkey pki/own/private/example_key.pem -in pki/own/certs/example_cert.pem
```

## Common Certificate Fields

### Country Codes (2-letter ISO codes)
- US - United States
- UK - United Kingdom
- CA - Canada
- DE - Germany
- FR - France
- JP - Japan
- AU - Australia
- CN - China
- IN - India
- BR - Brazil

### Common Name Examples
- **Domain:** `www.example.com`, `*.example.com` (wildcard)
- **IP Address:** `192.168.1.1`, `10.0.0.1`
- **Localhost:** `localhost`, `127.0.0.1`
- **Service:** `mail.example.com`, `api.example.com`
- **Person:** `John Doe`, `jane.smith@example.com`

### Organization Examples
- Company names: `Example Corporation`, `Acme Inc.`
- Departments: `IT Department`, `Development Team`
- Personal: `Personal`, `Self-Signed`

## Validity Period Guidelines

### Development/Testing
- **Short-term testing:** 30 days
- **Development environment:** 90-365 days
- **Staging environment:** 365 days

### Production (if applicable)
- **Server certificates:** 365-398 days (current industry standard)
- **Client certificates:** 365-730 days
- **Code signing:** 1-3 years
- **Root CA certificates:** 10-20 years
- **Intermediate CA certificates:** 5-10 years

**Note:** Modern browsers and security policies may reject certificates with validity longer than 398 days (13 months).

## PKI Directory Usage Guide

### own/certs/
Store certificates you've created or own:
- Self-signed certificates
- Your server certificates
- Your client certificates
- Certificates you've generated for your services

### own/private/
**CRITICAL - KEEP SECURE!**
Store private keys corresponding to your certificates:
- Never share these files
- Set appropriate file permissions (read-only for you)
- Backup securely
- Consider encryption at rest

### trusted/
Store certificates you explicitly trust:
- CA certificates you trust
- Partner organization certificates
- Third-party service certificates you verify and trust
- Manually verified certificates

### rejected/
Store certificates you explicitly don't trust:
- Certificates that failed verification
- Expired certificates you want to keep for reference
- Revoked certificates
- Suspicious or fraudulent certificates

### issuers/
Store Certificate Authority (CA) certificates:
- Root CA certificates
- Intermediate CA certificates
- Self-signed CA certificates you've created
- CA certificates from certificate chains

### crl/
Store Certificate Revocation Lists:
- Downloaded CRLs from CAs
- Your own CRLs if you're running a CA
- CRLs for verification purposes

## Sample Certificate Configurations

### 1. Local Development Server
```
Common Name: localhost
Organization: Development
Country: US
Validity: 365 days
Use: Local web server testing
```

### 2. Internal Network Server
```
Common Name: internal.company.local
Organization: Company IT
Country: US
Validity: 730 days
Use: Internal application server
```

### 3. Test API Server
```
Common Name: api.test.local
Organization: Development Team
Country: US
Validity: 90 days
Use: API development and testing
```

### 4. Certificate Authority
```
Common Name: Company Root CA
Organization: Company Security
Country: US
Validity: 3650 days (10 years)
Use: Signing other certificates
```

### 5. Email Encryption
```
Common Name: john.doe@example.com
Organization: Example Corp
Country: US
Validity: 730 days
Use: Email signing and encryption
```

## File Naming Conventions

### Recommended Naming Pattern
```
{identifier}_{type}.{ext}

Examples:
- localhost_cert.pem        # Certificate for localhost
- localhost_key.pem         # Private key for localhost
- example.com_cert.pem      # Certificate for example.com
- example.com_key.pem       # Private key for example.com
- rootca_cert.pem           # Root CA certificate
- rootca_key.pem            # Root CA private key
```

### Common Extensions
- `.pem` - PEM format (Base64 encoded, human-readable)
- `.crt` - Certificate (can be PEM or DER)
- `.cer` - Certificate (typically DER on Windows)
- `.key` - Private key
- `.der` - DER format (binary)
- `.pfx` or `.p12` - PKCS#12 format (certificate + private key bundle)
- `.csr` - Certificate Signing Request

## Security Best Practices

### Private Key Security
1. **File Permissions:** Restrict to owner read-only (chmod 400 on Unix)
2. **Encryption:** Consider encrypting keys with passphrase
3. **Backup:** Store encrypted backups in secure location
4. **Access Control:** Limit who can access private directory
5. **No Version Control:** Never commit private keys to Git

### Certificate Management
1. **Track Expiration:** Monitor certificate expiration dates
2. **Rotate Regularly:** Don't wait until last minute to renew
3. **Document Usage:** Keep record of where certificates are used
4. **Test Before Deployment:** Verify certificates in test environment
5. **Revocation Plan:** Have process for certificate revocation

### Development vs Production
1. **Different Keys:** Never use dev keys in production
2. **Shorter Validity:** Use shorter validity for dev certificates
3. **Clear Labeling:** Mark dev certificates clearly
4. **Separate Storage:** Keep dev and prod certificates separate
5. **Different CAs:** Consider separate CA hierarchies

## Integration Examples

### Using with IIS (Internet Information Services)
1. Convert to PFX format:
   ```
   openssl pkcs12 -export -out cert.pfx -inkey key.pem -in cert.pem
   ```
2. Import PFX in IIS Manager → Server Certificates → Import
3. Bind to site in Site Bindings → Add → HTTPS → Select certificate

### Using with Apache
```apache
<VirtualHost *:443>
    ServerName example.com
    SSLEngine on
    SSLCertificateFile    /path/to/pki/own/certs/example.com_cert.pem
    SSLCertificateKeyFile /path/to/pki/own/private/example.com_key.pem
</VirtualHost>
```

### Using with Nginx
```nginx
server {
    listen 443 ssl;
    server_name example.com;
    
    ssl_certificate     /path/to/pki/own/certs/example.com_cert.pem;
    ssl_certificate_key /path/to/pki/own/private/example.com_key.pem;
}
```

### Using with Docker
```dockerfile
FROM nginx:alpine
COPY pki/own/certs/example_cert.pem /etc/nginx/certs/
COPY pki/own/private/example_key.pem /etc/nginx/certs/
```

## Troubleshooting Common Issues

### Certificate-Key Mismatch
**Check if certificate and key match:**
```bash
# Get certificate modulus
openssl x509 -noout -modulus -in cert.pem | openssl md5

# Get key modulus
openssl rsa -noout -modulus -in key.pem | openssl md5

# If both MD5 hashes match, they're a pair
```

### Certificate Not Trusted
**Reasons:**
- Self-signed (not in trusted CA store)
- Expired
- Wrong Common Name (doesn't match domain)
- Invalid certificate chain

**Solutions:**
- Import to trusted certificate store
- Verify dates
- Check CN matches hostname
- Include intermediate certificates

### Permission Denied
**Unix/Linux:**
```bash
# Set correct permissions
chmod 600 pki/own/private/*.pem
chmod 644 pki/own/certs/*.pem
```

**Windows:**
```cmd
# Right-click → Properties → Security → Edit
# Remove unnecessary users, keep only your account
```

## Additional Resources

- **OpenSSL Cookbook:** https://www.feistyduck.com/books/openssl-cookbook/
- **X.509 Certificate RFC:** https://tools.ietf.org/html/rfc5280
- **PKI Basics:** https://en.wikipedia.org/wiki/Public_key_infrastructure
- **Certificate Transparency:** https://certificate.transparency.dev/

---

**Remember:** This application creates self-signed certificates suitable for development and testing. For production use with public-facing services, obtain certificates from a trusted Certificate Authority (CA) like Let's Encrypt, DigiCert, or GlobalSign.
