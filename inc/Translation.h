
//  ********************  TRANSLATION.H  ********************

void PackByte( unsigned char Code, unsigned char Data, BUFFER *buffer);
void packData( unsigned char Code, unsigned char Data, BUFFER *buffer);
void packDataEnd( unsigned char Code, unsigned char Data, BUFFER *buffer);
void CopyStringFlash( BUFFER *buffer, unsigned char __flash *string);
void CopyString( BUFFER *buffer, unsigned char *string);
void CopyStrDim1(unsigned char *pIn, unsigned char *pOut, unsigned char Row);
void CopyStrDim2(unsigned char *pIn, unsigned char *pOut, unsigned char Row, unsigned char Col);

void dateFormat(BUFFER *buffer, unsigned char __flash *stringDate);
void verFormat( BUFFER *buffer, unsigned char *string);

//  ********************  TRANSLATION.H  ********************

