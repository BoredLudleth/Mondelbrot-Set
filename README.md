# Оптимизация алгоритм множества Мандельброта #
“Not to be defeated is up to you, whether you win is up to the enemy.”
### Дисклеймер ###
Всем привет,
этот проект написан как учебное задание для курса И.Р.Дединского, с целью разобраться в теме SIMD (Single Instruction Multiple Data) и помочь интересующимся подробнее узнать эту тему.
Цель проекта - ускорение вычислений за счет параллельной обработки данных на уровне аппаратных команд.

Для визуализации картинки использовалась библиотека SFML.

Для ускорения программы я пользовался AVX2.

#### Характеристики моего ноутбука ####
OS: Ubuntu 22.04.1 LTS Jellyfish

Процессор: AMD Ryzen 5 4000 series

gcc version 11.3.0

## Компиляция ##
```
make link    - линковка проекта

make compile - компиляция проекта

make         - компиляция + линковка

./main       - для запуска картинки, в терминале будет текущий FPS 
```

## Множество Мандельброта ##
Для начала разберемся, что такое множество Мандельброта и зачем это оптимизировать?
Множество Мандельброта - это геометрический объект, который получается путем итерации специальной формулы в комплексной плоскости. Формула выглядит следующим образом:

$z_{n+1} = z_{n}^2 + c$

Где $z_{n}$ - комплексное число, $c$ - фиксированная точка комплексной плоскости.

Если в данной формуле последовательность $z_{n}$ не ограничена, то точка $c$ не принадлежит множеству Мандельброта. 
Если же последовательность $z_{n}$ ограничена, то точка $c$ принадлежит множеству Мандельброта.

Множество Мандельброта имеет сложную фрактальную структуру и представляет собой комплексную плоскость, на которой различные области соответствуют разным значениям $c$. 
Красивые фрактальные рисунки множества Мандельброта получаются путем визуализации этой плоскости.

В своей работе я пользовался несколько другой формулой, а именно:

>$x_{n+1} = x_{n}^2 - y_{n}^2 + x_{0}$

>$y_{n+1} = 2 * x_{n} * y_{n} + y_{0}$

Сложность вычисления каждого пикселя состоит в том, что мы не можем предугадать поведения следующей точки последовательности по изначальной координате,
поэтому нам приходится использовать рекурентную формулу.

## Реализация без SIMD ##
Написание алгоритма для высчитывания не является чем-то сложным. Ниже приведен код алгоритма без оптимизаций.

```C
void countPix (unsigned char* pixels, float x0, float y0, float length) {             // $x_{0}$ и $y_{0}$ - координаты левого верхнего угла 
    int counter = 0;                                                                  // подсчёт сколько точек в последоватеьлности, начиная с первой ограниченны 10
    int current_p = 0;                                                                // current_p - индекс в массиве на текущий пиксель 

    float x_2 = 0;
    float y_2 = 0;
    float xy = 0;

    float max_r = 10;                                                                 // $r^{2} - которым ограниченны точки последовательности$

    float dx = length / 1000;                                                         // Изменение координаты x и/или y при сдвиге на 1 пиксель 
    const float cx0 = x0;

    for (int pix_y = 0; pix_y < 1000; pix_y++) {
        y0 -= dx;
        x0 = cx0;
        for (int pix_x = 0; pix_x < 1000; pix_x++, current_p += 4) {
            x0 += dx;

            for (float x_n = x0, y_n = y0; counter < 255; counter++) {
                x_2 = x_n * x_n;
                y_2 = y_n * y_n;
                xy = x_n * y_n;

                if ((x_2 + y_2) >= max_r) {
                    break;
                } else {
                    x_n = x_2 - y_2 + x0; 
                    y_n = xy + xy + y0; 
                }
            }

            pixels[current_p] = 255 - counter;
            pixels[current_p + 1] = (counter%4) * 64;
            pixels[current_p + 2] = 128;
            pixels[current_p + 3] = 255 - counter;
            counter = 0;
        }
    }
}
    
```

Важно отметить, что в моей реализации, я ограничиваю координаты точек последовательностей круговой окрестностью с радиусом $10^{0.5}$ 
центром в начале координат (0,0). При увеличение "радиуса вылета" визуальных изменений не наблюдаось, поэтому решено было оставить именно такой параметр.

В результате обработки массива пикселей (для каждого пикселя четыре элемента массива: RGBA - первый 3 элемента отвечают за цвет, а последний за насыщенность).
Полученное изображение расположено на координатной плоскости $-3 < x < 3$ и $-3 < y < 3$. 

![image](https://user-images.githubusercontent.com/111277087/234153051-d5af9225-8591-4534-b3c1-a54af5489c2d.png)


Следует отметить, что по изображению можно двигаться
```
Движение по координатам X и Y осуществляется засчёт стрелочек. 

Для увеличение масштаба в 1.3 нажмите F1

Для уменьшения масштаба в 1.3 нажмите F2
```

##### Примечание: Уже сейчас можно заметить, что чем больше черных точек, тем медленне обновляется график.

Для чистоты эксперимента будем измерять FPS.

##### FPS (Frames per second) - это мера частоты кадров, используемая в компьютерных играх и видео. Она указывает, сколько кадров отображается в секунду на экране.

| Ключ оптимизации |        FPS       |
|------------------|------------------|
|       None       |        11        |
|       -O1        |        11        |
|       -O2        |        11        |
|       -O3        |        11        |
|       -Ofast     |        11        |

Промежуточные результаты показывают нам, что компилятор практически никак не оптимизирует наш код, поэтому попробуем оптимизировать наш код вручную...


## Реализация с AVX2 ##
Как я говорил раннее, оптимизацию будем проводить с помощью AVX2.

##### AVX2 (Advanced Vector Extensions 2) - это расширение для набора инструкций системных процессоров, разработанных компанией Intel, которое позволяет работать с векторными операциями.AVX2 поддерживает 256-битные векторные операции, что позволяет процессору выполнять одновременно несколько операций в одном цикле. Это дает значительное преимущество в производительности, особенно при обработке больших объемов данных.

Вплоне закономерным вопросом после этого будет за счёт какого основополагающего принципа будет происходить ускорение?

Ответом на этот вопрос будет SIMD, так чем она лучше д и почему в нашем случае она будет эффективнее?

Для обычной обработки все действия с некоторым регистром/блоком памяти происходят последовательно, как показано на рисунке, с использованием пузырьков, которые замедляют программу:
![image](https://user-images.githubusercontent.com/111277087/234156322-18b7cd0a-bf97-4540-9abe-596dc1a4831a.png)

Однако при конвеерной обработке вся комбинаторная логика делится на несколько упорядоченных этапов с меньшим количество пузырьков. После выполнения некоторого этапа промежуточные результаты помещаются во временыые регистры.
Из временных регистров данные попадают на следующий этап комбинаторной логики, а на предыдущий поступает новое значение. Тем самым наши вычисления происходят параллельно, тем самым повышая быстродействие программы.
![image](https://user-images.githubusercontent.com/111277087/234156969-f31d5407-e192-4384-85ea-d7181ebb649d.png)

Получается, что конвеерная обработка очень хорошо подходит для обработки больших блоков данных, над которыми надо провести одни и те же операции, что соответствует нашей задаче.

Но есть ещё не менее важное условие для эффективного применения комбинаторная логика на каждом шаге должна быть тоже довольно-таки эффективной.
С этим нам могут помочь интринсики.

##### Интринсики (или встроенные функции) в программировании - это специальные операции, реализованные на уровне аппаратного обеспечения компьютера, которые могут выполнять определенные задачи быстрее, чем обычный код, написанный на языке программирования.


```C
void countPix (unsigned char* pixels, float x0, float y0, float length) {
    int counter = 0;
    int current_p = 0;

    float max_r = 10;

    float dx = length / 1000;

    const float cx0 = x0;

    __m256 _maxr = _mm256_set_ps (DUP8(max_r));

    __m256 _delta = _mm256_set_ps (0, dx, 2 * dx, 3 * dx, 4 * dx, 5 * dx, 6 * dx, 7 * dx); 

    __m256 _x0 = _mm256_set_ps (DUP8(x0));
    _x0 = _mm256_add_ps (_x0, _delta);                            // Мы задали начальные координаты точек
    __m256 _y0 = _mm256_set_ps (DUP8(y0));                        // Если вы захотите повторить подобный проект, то подберите масштаб картинки так, чтобы в ширине,
    __m256 _xn = _mm256_set_ps (DUP8(x0));                        // кол-во точек делилось на 8, иначе по краям у вас могут появится непредвиденные пиксели или произойдет Segmentation fault
    _xn = _mm256_add_ps (_xn, _delta);
    __m256 _yn = _mm256_set_ps (DUP8(y0));

    __m256 _x2 = _mm256_mul_ps (_xn, _xn);
    __m256 _y2 = _mm256_mul_ps (_yn, _yn);
    __m256 _xy = _mm256_mul_ps (_xn, _yn);

    __m256 _counter = _mm256_setzero_ps ();                       // Подсчёт сколько первых членов  лежит в круговой окрестности около начала координат

    __m256 _increase = _mm256_set_ps (DUP8(1));

    __m256 _rcur = _mm256_setzero_ps ();

    __m256 _mask = _mm256_cmp_ps (_rcur, _maxr, _CMP_LE_OS);      // Маска, имеет размер 4 байта для каждого элемента. Если элемент ещё не вылетил за круг, то она имеет вид 11..11, иначе 00..00
    
    for (int pix_y = 0; pix_y < 1000; pix_y++) {
        y0 -= dx;
        x0 = cx0;
        for (int pix_x = 0; pix_x < 1000; pix_x += 8) {
            _x0 = _mm256_set_ps (DUP8(x0));
            _x0 = _mm256_add_ps (_x0, _delta);
            _y0 = _mm256_set_ps  (DUP8(y0));

            _xn = _mm256_set_ps (DUP8(x0));
            _xn = _mm256_add_ps (_xn, _delta);
            _yn = _mm256_set_ps  (DUP8(y0));

            _counter = _mm256_setzero_ps ();

            _mask = _mm256_cmp_ps (_increase, _maxr, _CMP_LE_OS); // Сравнение расстояния между началом координат и текущими точками и расстоянием вылета

            counter = 0;
            
            while (counter < 255 && (mask_count (_mask) != 0)) {  // Подсчёт следующих элементов последовательности, при условии, что все предыдущие находятся в пределах окружности вокруг начала координат
                _x2 = _mm256_mul_ps (_xn, _xn);
                _y2 = _mm256_mul_ps (_yn, _yn);
                _xy = _mm256_mul_ps (_xn, _yn);

                _xn = _mm256_sub_ps (_x2, _y2);
                _xn = _mm256_add_ps (_xn, _x0);

                _yn = _mm256_add_ps (_xy, _xy);
                _yn = _mm256_add_ps (_yn, _y0);

                _rcur = _mm256_add_ps (_mm256_mul_ps (_xn, _xn), _mm256_mul_ps (_yn, _yn));

                _mask = _mm256_cmp_ps (_rcur, _maxr, _CMP_LE_OS);

                _counter = _mm256_add_ps (_counter, _mm256_and_ps (_mask, _increase));

                counter++;
            }

            current_p += 32;
            x0 += 8 * dx;

            float* count = (float*) &_counter;

            for (int i = 0; i < 8; i++) {                       // Передача в массив пикселей
                pixels[current_p  - i * 4] = 255 - (int) count[i];
                pixels[current_p  - i * 4 + 1] = (((int) count[i])%4) * 64;
                pixels[current_p  - i * 4  + 2] = 128;
                pixels[current_p  - i * 4 + 3] = 255 - (int) count[i];
            }
        }   
    }
}

```

![image](https://user-images.githubusercontent.com/111277087/234160240-80072b51-329b-436c-83c6-004cb1b83908.png)

Этот вариант работает намного быстрее своего собрата, что мы можем наблюдать по скорости отрисовки изображений с большим количеством точек, принадлежащих множеству

| Ключ оптимизации |        FPS       |
|------------------|------------------|
|       None       |        12        |
|       -O1        |        73        |
|       -O2        |        105       |
|       -O3        |        109       |
|       -Ofast     |        120       |

## Выводы ##
Наш компилятор ещё не идеален, он не смог сам оптимизировать нашу программу, тем не менее, мы смогли ускорить нашу программу примерно в 10-11 раз. 
Также мы смогли убедиться, что ключи оптимизации отличаются и наиболее эффективными из них были -O3 и -Ofast.

Благодарю за прочтение, надеюсь моя readme смогла немного прояснить SIMD-оптимизацию.

src: Рэндал О.Брайант, Дэвид О.Халларон "Компьютерные сети. Архитектура и программирование"






