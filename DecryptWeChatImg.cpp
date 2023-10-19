#include <stdio.h>
#include <malloc.h>

unsigned char getXorKey(unsigned short tmp)
{
    if( (tmp >> 8) == (tmp & 0xff)){
        //Hight byte   Low byte
        unsigned char xorKey = tmp & 0xff;
        return xorKey;
    }else{
        return 0;
    }
}
int main(int argc,char *argv[])
{
    if(argc<2){
        printf("param error!\n");
        return 0;
    }
    char *datPath = argv[1];

    //xor with the first tow bytes
    FILE *fp = fopen(datPath,"rb");
    fseek(fp,SEEK_SET,SEEK_END);
    int len = ftell(fp);
    unsigned char * buf = (unsigned char*)malloc(len);
    fseek(fp,SEEK_SET,SEEK_SET);
    fread(buf,len,1,fp);
    fclose(fp);
    //JPG -> FF D8
    unsigned short tmp = *(unsigned short*)buf ^ 0xD8FF;
    unsigned char xorKey =  getXorKey(tmp);
    if(xorKey==0){
    //PNG -> 89 50
        tmp = *(unsigned short*)buf ^ 0x5089;
        xorKey = getXorKey(tmp);
    }
    if(xorKey==0){
        printf("xor key error!\n");
        return 0;
    }

    //Decrypt Wechat Image
    for(int i=0;i<len;i++){
        buf[i] ^= xorKey;
    }
    //test demo
    fp = fopen("test.png","wb");
    fwrite(buf,len,1,fp);
    fclose(fp);
    printf("Decrypt finish!\n");

    return 0;
}