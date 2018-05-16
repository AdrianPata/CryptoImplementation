package ro.pata.crypto;

import org.apache.commons.io.FileUtils;

import javax.crypto.*;
import javax.crypto.spec.DESKeySpec;
import java.io.File;
import java.io.IOException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.spec.InvalidKeySpecException;

public class DESTest {
    public static void main(String[] args) throws NoSuchAlgorithmException, IOException, InvalidKeyException, InvalidKeySpecException, NoSuchPaddingException, BadPaddingException, IllegalBlockSizeException {
        File keyFile=new File("des.key");
        File sourceFile=new File("des_source.txt");
        File encFile=new File("des_enc_java.bin");
        File decFile=new File("des_dec_java.txt");
        SecretKey myDesKey;

        if(!keyFile.exists()){
            KeyGenerator keygenerator = KeyGenerator.getInstance("DES");
            myDesKey = keygenerator.generateKey();
            FileUtils.writeByteArrayToFile(keyFile,  myDesKey.getEncoded());
        } else {
            DESKeySpec dks = new DESKeySpec(FileUtils.readFileToByteArray(keyFile));
            SecretKeyFactory skf = SecretKeyFactory.getInstance("DES");
            myDesKey=skf.generateSecret(dks);
        }

        Cipher desCipher= Cipher.getInstance("DES/ECB/PKCS5Padding");

        desCipher.init(Cipher.ENCRYPT_MODE, myDesKey);
        byte[] text =FileUtils.readFileToByteArray(sourceFile);
        byte[] textEncrypted = desCipher.doFinal(text);
        FileUtils.writeByteArrayToFile(encFile,textEncrypted);

        desCipher.init(Cipher.DECRYPT_MODE, myDesKey);
        byte[] textDecrypted = desCipher.doFinal(FileUtils.readFileToByteArray(encFile));
        FileUtils.writeByteArrayToFile(decFile,textDecrypted);
    }
}
