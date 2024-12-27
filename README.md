
# مشروع NXP Cup (TFC Model)

## الوصف (Description)

هذا المشروع يهدف إلى تصميم وتطوير برنامج لنموذج سيارة ذاتية القيادة يتم التحكم فيها بواسطة لوحة FRDM-KL25Z، وذلك للمشاركة في مسابقة NXP Cup. 
(The project aims to design and develop software for a self-driving car model controlled by the FRDM-KL25Z board, for participation in the NXP Cup competition.)

The project aims to design and develop software for a self-driving car model controlled by the FRDM-KL25Z board, for participation in the NXP Cup competition.

---

## الملفات الرئيسية (Main Files)

- `main.c` (ملف يحتوي على المنطق الأساسي للتحكم في السيارة الذاتية القيادة.)
  (A file containing the main logic for controlling the self-driving car.)
- `mainV0.c` (إصدار أولي لملف التحكم.)
  (An initial version of the control file.)
- `mainV2.c` (إصدار محدث يحتوي على تحسينات.)
  (An updated version with improvements.)
- `sa_mtb.c` (ملف خاص بالتعامل مع الوظائف المحيطية مثل التتبع.)
  (A file dedicated to handling peripheral functions such as tracing.)

---

## الملفات الإضافية (Additional Files)

- `ReferencedRSESystems.xml` (ملف إعدادات النظام المرتبطة بلوحة FRDM-KL25Z.)
  (A configuration file for system settings linked to the FRDM-KL25Z board.)
- `cwGeneratedFileSetLog.txt` (ملف يسرد جميع الملفات التي يتم إنشاؤها عند البناء.)
  (A file listing all the generated files during the build process.)
- `project.txt` (ملف إعدادات المشروع لتحديد كيفية بناء المشروع وتشغيله.)
  (A project configuration file defining how to build and run the project.)

---

## الوثائق المرجعية (Reference Documents)

- `KL25P80M48SF0.pdf` (ورقة بيانات شاملة للوحة KL25Z.)
  (A comprehensive datasheet for the KL25Z board.)
- `KL25P80M48SF0RM.pdf` (دليل مرجعي يحتوي على التفاصيل التقنية الدقيقة للوحة KL25Z.)
  (A reference manual with precise technical details of the KL25Z board.)

---

## إعداد المشروع (Project Setup)

1. تأكد من تثبيت Eclipse IDE وCodeWarrior.
   (Ensure that Eclipse IDE and CodeWarrior are installed.)

2. قم بفتح المشروع من خلال Eclipse IDE.
   (Open the project using Eclipse IDE.)

3. استخدم أداة OpenSDA لتوصيل اللوحة FRDM-KL25Z بجهاز الكمبيوتر.
   (Use the OpenSDA tool to connect the FRDM-KL25Z board to your computer.)

4. قم ببناء المشروع باستخدام الأمر `Build` في Eclipse.
   (Build the project using the `Build` command in Eclipse.)

5. حمل البرنامج إلى اللوحة باستخدام Debugger.
   (Upload the program to the board using the Debugger.)

---

## تكامل ULTRASONIC (ULTRASONIC Integration):
[https://github.com/elbasri/ultrasonic-kl25z-nxp](https://github.com/elbasri/ultrasonic-kl25z-nxp)
(رابط إلى مستودع GitHub الخاص بتكامل مستشعر الموجات فوق الصوتية مع لوحة KL25Z.)
(Link to the GitHub repository for ultrasonic sensor integration with the KL25Z board.)


## المساهمون (Contributors)
عبد الناصر البصري (Abdennacer Elbasri)
- **Twitter**: [@abdennacerelb](https://twitter.com/abdennacerelb)  
- **LinkedIn**: [@elbasri](https://linkedin.com/in/elbasri)  
