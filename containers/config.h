#ifndef CONFIG_H
#define CONFIG_H

#include "../utils/exceptions/invalidvalueexception.h"

/**
 * @file CardPositionData.h
 *
 * @brief This class is responsible for storing
 * the config what is used during generation to decide
 * how much image should be generated.
 *
 * @author Domonkos Say
 *
 */

class Config
{
    unsigned image_count = 0;
    unsigned normal_image_count = 0;
    unsigned augment_count = 0;
    unsigned augment_noise = 0;
    unsigned augment_lens_blur = 0;
    unsigned augment_bilateral_blur = 0;
    unsigned augment_image_flip = 0;
    unsigned augment_cut_out = 0;
    unsigned augment_brightness = 0;
    unsigned augment_rgb_shift = 0;
public:
    /** @brief Creates an empty Config object.
     *  This object stores the loaded config.
     *  Use the setters to set the read values
     *  and the getters to access them.
     *  Use the validate function to validate a
     *  config file.
    */
    Config(){};

    /** @brief A getter for the image count config*/
    unsigned int get_image_count() const;
    /** @brief A setter for the image count config
     *
     *  @param new_image_count: a number representing the total
     *  number of images
    */
    void set_image_count(const unsigned int& new_image_count);


    /** @brief A getter for the normal image (not augmented) count config*/
    unsigned int get_normal_image_count() const;
    /** @brief A setter for the normal image (not augmented) count config
     *
     *  @param new_normal_image_count: a number representing the total
     *  number of not augmented images
    */
    void set_normal_image_count(const unsigned int& new_normal_image_count);


    /** @brief A getter for the image augment count config*/
    unsigned int get_augment_count() const;
    /** @brief A setter for the image augment count config
     *
     *  @param new_augment_count: a number representing the total
     *  number of augments
    */
    void set_augment_count(const unsigned int& new_augment_count);


    /** @brief A getter for the noise augment count config*/
    unsigned int get_augment_noise() const;
    /** @brief A setter for the noise augment count config
     *
     *  @param new_augment_noise: a number representing the total
     *  number of noise augment operation to be done
    */
    void set_augment_noise(const unsigned int& new_augment_noise);


    /** @brief A getter for the lens blur augment count config*/
    unsigned int get_augment_lens_blur() const;
    /** @brief A setter for the lens blur augment count config
     *
     *  @param new_augment_lens_blur: a number representing the total
     *  number of lens blur augment operation to be done
    */
    void set_augment_lens_blur(const unsigned int& new_augment_lens_blur);


    /** @brief A getter for the bilateral blur augment count config*/
    unsigned int get_augment_bilateral_blur() const;
    /** @brief A setter for the bilateral blur augment count config
     *
     *  @param new_augment_bilateral_blur: a number representing the total
     *  number of bilateral blur augment operation to be done
    */
    void set_augment_bilateral_blur(const unsigned int& new_augment_bilateral_blur);


    /** @brief A getter for the image flip augment count config*/
    unsigned int get_augment_image_flip() const;
    /** @brief A setter for the image flip augment count config
     *
     *  @param new_augment_image_flip: a number representing the total
     *  number of image flip augment operation to be done
    */
    void set_augment_image_flip(const unsigned int& new_augment_image_flip);


    /** @brief A getter for the cut out augment count config*/
    unsigned int get_augment_cut_out() const;
    /** @brief A setter for the cut out augment count config
     *
     *  @param new_augment_cut_out: a number representing the total
     *  number of cut out augment operation to be done
    */
    void set_augment_cut_out(const unsigned int& new_augment_cut_out);


    /** @brief A getter for the brightness adjustion augment count config*/
    unsigned int get_augment_brightness() const;
    /** @brief A setter for the brightness adjustion augment count config
     *
     *  @param new_augment_brightness: a number representing the total
     *  number of brightness adjustion augment operation to be done
    */
    void set_augment_brightness(const unsigned int& new_augment_brightness);


    /** @brief A getter for the rgb shift augment count config*/
    unsigned int get_augment_rgb_shift() const;
    /** @brief A setter for the rgb shift augment count config
     *
     *  @param new_augment_rgb_shift: a number representing the total
     *  number of rgb shift augment operation to be done
    */
    void set_augment_rgb_shift(const unsigned int& new_augment_rgb_shift);


    /** @brief Checks if the input config file was valid.
     *  It checks that the total image count is equal to
     *  the normal image count + augment count or not.
     *
     *  It checks that the invidual augments adds up
     *  to the total number of augments or not.
     *
     *  @throws Utils::Exceptions::InvalidValueException
     *  if either case was false so the config file is invalid.
    */
    bool validate() const;
};

#endif // CONFIG_H
