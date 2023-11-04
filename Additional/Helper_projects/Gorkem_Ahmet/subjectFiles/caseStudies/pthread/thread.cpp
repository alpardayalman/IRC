#include <iostream>
#include <pthread.h>
#include <unistd.h>

void* thread_function(void* arg) {
    int thread_id = *(int*)arg; // Argümanı al
    for (int i = 1; i <= 5; ++i) {
        std::cout << "İş Parçacığı " << thread_id << " çalışıyor... Adım " << i << std::endl;
        sleep(1);
    }
    std::cout << "İş Parçacığı " << thread_id << " tamamlandı." << std::endl;
    return NULL;
}

int main() {
    const int num_threads = 3;
    pthread_t threads[num_threads];

    for (int i = 0; i < num_threads; ++i) {
        int thread_id = i + 1;
        int result = pthread_create(&threads[i], NULL, thread_function, &thread_id);
        if (result != 0) {
            std::cerr << "İş parçacığı oluşturma hatası" << std::endl;
            return 1;
        }

        // result = pthread_join(threads[i], NULL);
        // if (result != 0) {
        //     std::cerr << "İş bekletme hatası" << std::endl;
        //     return 1;
        // }

        result = pthread_detach(threads[i]);
        if (result != 0) {
            std::cerr << "İş parçacığını ayırma hatası" << std::endl;
            return 1;
        }
    }

    // Ana iş parçacığı devam ediyor
    // ...

    sleep(6); // Ana iş parçacığının bitmeden önce beklemesi için

    return 0;
}

// pthread_create: Bu fonksiyon, yeni bir thread oluşturur. Fonksiyonun ilk parametresi, oluşturulan thread’in tanımlayıcısını (ID) tutacak bir değişkenin adresini alır. İkinci parametresi, thread’in özniteliklerini belirleyen bir yapıyı (attribute) alır. Üçüncü parametresi, thread’in çalıştıracağı fonksiyonu belirtir. Dördüncü parametresi ise, thread’in çalıştıracağı fonksiyona geçirilecek parametreleri belirtir1.
// pthread_join: Bu fonksiyon, belirtilen thread’in sonlanmasını bekler ve sonlandığında thread’in dönüş değerini alır. Fonksiyonun ilk parametresi, beklenen thread’in tanımlayıcısını (ID) alır. İkinci parametresi ise, thread’in dönüş değerini tutacak bir değişkenin adresini alır1.
// pthread_exit: Bu fonksiyon, çağrıldığı thread’i sonlandırır ve belirtilen dönüş değerini döndürür. Fonksiyonun tek parametresi, thread’in dönüş değerini belirtir1.
// pthread_detach: Bu fonksiyon, belirtilen thread’i ayrılmış (detached) olarak işaretler. Bir thread ayrıldığında, bu thread sonlandığında kaynakları otomatik olarak sistem tarafından geri alınır ve başka bir thread’in sonlanan thread ile birleşmesine (join) gerek kalmaz1.