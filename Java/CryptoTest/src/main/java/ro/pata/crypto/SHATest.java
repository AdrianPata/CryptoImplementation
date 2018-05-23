package ro.pata.crypto;

import org.apache.commons.io.FileUtils;

import java.io.File;
import java.io.IOException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;

public class SHATest {
    public static void main(String[] args) throws IOException, NoSuchAlgorithmException {
        File shaSrc=new File("sha_src.txt");
        File shaDest=new File("sha_dest.txt");
        byte[] src=FileUtils.readFileToByteArray(shaSrc);
        byte[] dest=FileUtils.readFileToByteArray(shaDest);

        MessageDigest digest = MessageDigest.getInstance("SHA-256");
        byte[] hash = digest.digest(src);

        if(Arrays.equals(hash,dest)){
            System.out.println("Test OK");
        } else {
            System.out.println("Test failed");
        }
    }
}
