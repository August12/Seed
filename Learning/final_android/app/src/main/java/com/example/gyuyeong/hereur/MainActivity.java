package com.example.gyuyeong.hereur;

import android.content.Context;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Vibrator;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import okhttp3.MediaType;
import okhttp3.MultipartBody;
import okhttp3.OkHttpClient;
import okhttp3.RequestBody;
import okhttp3.ResponseBody;
import okhttp3.logging.HttpLoggingInterceptor;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.http.Multipart;
import retrofit2.http.POST;
import retrofit2.http.Part;



public class MainActivity extends AppCompatActivity {

    ImageView imageView = null;
    ImageView imageView2 = null;
    int serverResponseCode = 0;
    String upLoadServerUri = null;
   // ProgressDialog dialog = null;

    int cnt = 0;

    /**********  File Path *************/
    String uploadFilePath = "storage/emulated/0/vrecorder/" ;//Environment.getExternalStorageDirectory().getAbsolutePath() + "/vrecorder";
    String uploadFileName = "record"+ cnt +".wav";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);
        ImageButton recordBtn = (ImageButton) findViewById(R.id.recordBtn);

        ImageButton recordStop = (ImageButton) findViewById(R.id.recordStop);
        imageView = (ImageView) findViewById(R.id.imageView);
        imageView2 = (ImageView) findViewById(R.id.imageView2);

        imageView.setImageResource(R.drawable.ear);
        imageView2.setImageResource(R.drawable.text1);






        recordBtn.setOnClickListener(new View.OnClickListener() { //
            @Override
            public void onClick(View v) {

                imageView.setImageResource(R.drawable.wave);
                imageView2.setImageResource(R.drawable.text2);

                final WavRecorder wavRecorder = new WavRecorder(uploadFilePath+""+uploadFileName);
                wavRecorder.startRecording();

                new Handler().postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        //WavRecorder wavRecorder1 = new WavRecorder(uploadFilePath+""+uploadFileName);
                        wavRecorder.stopRecording();
                        uploadFile(uploadFilePath + "" + uploadFileName);
                        cnt++;
                        uploadFileName = "record"+ cnt +".wav";
                        Log.i("test", String.valueOf(cnt));
                    }
                },5000);



                new Handler().postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        imageView.setImageResource(R.drawable.ear);
                        imageView2.setImageResource(R.drawable.text1);
                    }
                },15000);


            }




        });


        upLoadServerUri = "http://172.30.1.37:5000/ggyu.php"  ;//"http://172.30.1.52:8080/test.php";

        recordStop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {//정지버튼
                //imageView.setImageResource(R.drawable.ear);
                //imageView2.setImageResource(R.drawable.text1);

               // final Dialog dialog2 = new Dialog(MainActivity.this);

                TextView tv = new TextView(MainActivity.this);
                tv.setText(" ~ 소리입니다");


                WavRecorder wavRecorder = new WavRecorder(uploadFilePath+""+uploadFileName);
                wavRecorder.stopRecording();


                
              /* new Thread(new Runnable() {
                    public void run() {
                        runOnUiThread(new Runnable() {
                            public void run() {
                               // makeText(getApplicationContext(), "업로드 중입니다" , LENGTH_LONG).show();
                            }
                        });
                        uploadFile(uploadFilePath + "" + uploadFileName);
                        cnt++;
                    }
                }).start();*/
            }
        });
    }


    public interface FileUploadService {
        @Multipart
        @POST("upload")
        Call<ResponseBody> upload(
                @Part ("description") RequestBody description,
                @Part MultipartBody.Part file
        );
    }


    public void uploadFile(String fileUri) {
        HttpLoggingInterceptor interceptor = new HttpLoggingInterceptor();
        interceptor.setLevel(HttpLoggingInterceptor.Level.BODY);
        OkHttpClient client = new OkHttpClient.Builder().addInterceptor(interceptor).build();

        FileUploadService service = new Retrofit.Builder().baseUrl("http://172.20.10.3:5000/").client(client).build().create(FileUploadService.class);


        File file = new File(fileUri);

       RequestBody requestFile = RequestBody.create(MediaType.parse("multipart/form-data"), file);

        // MultipartBody.Part is used to send also the actual file name
        final MultipartBody.Part body =
                MultipartBody.Part.createFormData("file", file.getName(), requestFile);

        // add another part within the multipart request
        final String descriptionString = "hello, this is description speaking";
        final RequestBody description =
                RequestBody.create(
                        okhttp3.MultipartBody.FORM, descriptionString);
        final String c = "a";

        // finally, execute the request
        Call<ResponseBody> call = service.upload(description, body);
        call.enqueue(new Callback<ResponseBody>() {
            @Override
            public void onResponse(Call<ResponseBody> call,
                                   Response<ResponseBody> response) {
                Log.v("Upload", "success");

                if (response.isSuccessful()) {
                    ResponseBody result = response.body();
                    try {
                        String resultString = result.string();
                        Log.i("mini",resultString);


                        JSONObject json = new JSONObject(resultString);

                        //int a = json.getInt("classfication");
                        int a = json.optInt("classification", 0);

                        Log.i("mini2", String.valueOf(a));

                        Vibrator vibrator = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
                       // AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(MainActivity.this);

                        // 제목셋팅

                        if(a ==0){
                            vibrator.vibrate(1000);
                            imageView.setImageResource(R.drawable.baby);
                            imageView2.setImageResource(R.drawable.crying);
                        }
                        else if(a==1){
                            vibrator.vibrate(1000);
                            imageView.setImageResource(R.drawable.dog);
                            imageView2.setImageResource(R.drawable.dogsound);
                        }
                        else if(a==2){
                            vibrator.vibrate(1000);
                            imageView.setImageResource(R.drawable.window);
                            imageView2.setImageResource(R.drawable.crack);
                        }
                        else if(a==3){
                            vibrator.vibrate(1000);
                            imageView.setImageResource(R.drawable.sound);
                            imageView2.setImageResource(R.drawable.siren);

                        }


                    } catch (IOException e) {
                        e.printStackTrace();
                    } catch (JSONException e) {
                        e.printStackTrace();
                    }

                }


            }

            @Override
            public void onFailure(Call<ResponseBody> call, Throwable t) {Log.e("Upload error:", t.getMessage());
            }
        });
    }




    public class WavRecorder {
        private static final int RECORDER_BPP = 16;
        private static final String AUDIO_RECORDER_FOLDER = "AudioRecorder";
        private static final String AUDIO_RECORDER_TEMP_FILE = "record_temp.raw";
        private static final int RECORDER_SAMPLERATE = 44100;
        private static final int RECORDER_CHANNELS = AudioFormat.CHANNEL_IN_MONO;
        private static final int RECORDER_AUDIO_ENCODING = AudioFormat.ENCODING_PCM_16BIT;
        short[] audioData;

        private AudioRecord recorder = null;
        private int bufferSize = 0;
        private Thread recordingThread = null;
        private boolean isRecording = false;
        int[] bufferData;
        int bytesRecorded;

        private String output;

        public WavRecorder(String path) {
            bufferSize = AudioRecord.getMinBufferSize(RECORDER_SAMPLERATE,
                    RECORDER_CHANNELS, RECORDER_AUDIO_ENCODING) * 3;

            audioData = new short[bufferSize]; // short array that pcm data is put
            // into.
            output = path;

        }

        private String getFilename() {
            return (output);
        }

        private String getTempFilename() {
            String filepath = Environment.getExternalStorageDirectory().getPath();
            File file = new File(filepath, AUDIO_RECORDER_FOLDER);

            if (!file.exists()) {
                file.mkdirs();
            }

            File tempFile = new File(filepath, AUDIO_RECORDER_TEMP_FILE);

            if (tempFile.exists())
                tempFile.delete();

            return (file.getAbsolutePath() + "/" + AUDIO_RECORDER_TEMP_FILE);
        }

        public void startRecording() {

            recorder = new AudioRecord(MediaRecorder.AudioSource.MIC,
                    RECORDER_SAMPLERATE, RECORDER_CHANNELS,
                    RECORDER_AUDIO_ENCODING, bufferSize);
            int i = recorder.getState();
            if (i == 1)
                recorder.startRecording();

            isRecording = true;

            recordingThread = new Thread(new Runnable() {
                @Override
                public void run() {
                    writeAudioDataToFile();
                }
            }, "AudioRecorder Thread");

            recordingThread.start();
        }

        private void writeAudioDataToFile() {
            byte data[] = new byte[bufferSize];
            String filename = getTempFilename();
            FileOutputStream os = null;

            try {
                os = new FileOutputStream(filename);
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            }

            int read = 0;
            if (null != os) {
                while (isRecording) {
                    read = recorder.read(data, 0, bufferSize);
                    if (read > 0) {
                    }

                    if (AudioRecord.ERROR_INVALID_OPERATION != read) {
                        try {
                            os.write(data);
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                }

                try {
                    os.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

        public void stopRecording() {
            if (null != recorder) {
                isRecording = false;

                int i = recorder.getState();
                if (i == 1)
                    recorder.stop();
                recorder.release();

                recorder = null;
                recordingThread = null;
            }

            copyWaveFile(getTempFilename(), getFilename());
            deleteTempFile();
            Log.i("test777",uploadFileName);
        }

        private void deleteTempFile() {
            File file = new File(getTempFilename());
            file.delete();
        }

        private void copyWaveFile(String inFilename, String outFilename) {
            FileInputStream in = null;
            FileOutputStream out = null;
            long totalAudioLen = 0;
            long totalDataLen ;
            long longSampleRate = RECORDER_SAMPLERATE;
            int channels = ((RECORDER_CHANNELS == AudioFormat.CHANNEL_IN_MONO) ? 1 : 2);
            long byteRate = RECORDER_BPP * RECORDER_SAMPLERATE * channels / 8;

            byte[] data = new byte[bufferSize];

            try {
                in = new FileInputStream(inFilename);
                out = new FileOutputStream(outFilename);
                totalAudioLen = in.getChannel().size();
                totalDataLen = totalAudioLen + 36;
                Log.i("test111",inFilename);


                WriteWaveFileHeader(out, totalAudioLen, totalDataLen, longSampleRate, channels, byteRate);

                while (in.read(data) != -1) {
                    out.write(data);
                }

                in.close();
                out.close();
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        private void WriteWaveFileHeader(FileOutputStream out, long totalAudioLen,
                                         long totalDataLen, long longSampleRate, int channels, long byteRate)
                throws IOException {
            byte[] header = new byte[44];

            header[0] = 'R'; // RIFF/WAVE header
            header[1] = 'I';
            header[2] = 'F';
            header[3] = 'F';
            header[4] = (byte) (totalDataLen & 0xff);
            header[5] = (byte) ((totalDataLen >> 8) & 0xff);
            header[6] = (byte) ((totalDataLen >> 16) & 0xff);
            header[7] = (byte) ((totalDataLen >> 24) & 0xff);
            header[8] = 'W';
            header[9] = 'A';
            header[10] = 'V';
            header[11] = 'E';
            header[12] = 'f'; // 'fmt ' chunk
            header[13] = 'm';
            header[14] = 't';
            header[15] = ' ';
            header[16] = 16; // 4 bytes: size of 'fmt ' chunk
            header[17] = 0;
            header[18] = 0;
            header[19] = 0;
            header[20] = 1; // format = 1
            header[21] = 0;
            header[22] = (byte) channels;
            header[23] = 0;
            header[24] = (byte) (longSampleRate & 0xff);
            header[25] = (byte) ((longSampleRate >> 8) & 0xff);
            header[26] = (byte) ((longSampleRate >> 16) & 0xff);
            header[27] = (byte) ((longSampleRate >> 24) & 0xff);
            header[28] = (byte) (byteRate & 0xff);
            header[29] = (byte) ((byteRate >> 8) & 0xff);
            header[30] = (byte) ((byteRate >> 16) & 0xff);
            header[31] = (byte) ((byteRate >> 24) & 0xff);
            header[32] = (byte) (((RECORDER_CHANNELS == AudioFormat.CHANNEL_IN_MONO) ? 1
                    : 2) * 16 / 8); // block align
            header[33] = 0;
            header[34] = RECORDER_BPP; // bits per sample
            header[35] = 0;
            header[36] = 'd';
            header[37] = 'a';
            header[38] = 't';
            header[39] = 'a';
            header[40] = (byte) (totalAudioLen & 0xff);
            header[41] = (byte) ((totalAudioLen >> 8) & 0xff);
            header[42] = (byte) ((totalAudioLen >> 16) & 0xff);
            header[43] = (byte) ((totalAudioLen >> 24) & 0xff);

            out.write(header, 0, 44);
        }

    }




}




