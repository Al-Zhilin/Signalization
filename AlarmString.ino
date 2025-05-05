String AlarmString(byte comm, byte num) {    // 0 - свет, 1 - датчики движения, 2 - пожарные датчики, 3 - Date(1).
  if (!comm)  {
    switch (num)  {   // 0 - вкл свет, 1 - откл свет
      case 0:
        return F("Включили свет! \n");

      case 1:
        return String("Отключили свет! \n");

      default:
        return String("err0 \n");
    }
  }

  if (comm == 1)  {
    switch (num)  {   //0 - дом, 1 - участок №1, т.д
      case 0:
        return String("Кто-то в Доме! \n");

      case 1:
        return String("Кто-то на участке №1! \n");
      
      case 2:
        return String("Кто-то на участке №2! \n");
      
      case 3:
        return String("Кто-то на участке №3! \n");
       
      default:
        return String("err1 \n");
    }
  }

  if (comm == 2)  {   //0 - сброс датчиков, 1 - включили датчики, 2 - отключили датчики, 3 - внутренний сработал, 4 - наружний сработал
    switch (num)  {
      case 0:
        return String("Пожарные датчики сброшены! \n");

      case 1:
        return String("Пожарные датчики включены! \n");

      case 2:
        return String("Пожарные датчики отключены! \n");

      case 3:
        return String("Внутренний пож. датчик сработал! \n");

      case 4:
        return String("Наружный пож. датчик сработал! \n");

      default:
        return String("err2 \n");
    }
  }

  if (comm == 3)  {
    return Date(1);
  }

  return "/err/";
}