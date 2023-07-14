#ifndef TEMPLATECONTAINER_H
#define TEMPLATECONTAINER_H

#include "image.h"
#include "../utils/random.h"
#include "../utils/exceptions/invalidtemplateexception.h"
#include "../utils/exceptions/novalidinputsleftexception.h"
#include "../utils/exceptions/badusageexception.h"

/**
 * @file TemplateContainer.h
 *
 * @brief This class is responsible for storing
 * how much operation is left on an image from
 * a given type.
 *
 * @author Domonkos Say
 *
 */

class TemplateContainer
{
    QVector<QPair<Image*, unsigned int>> templates;
    unsigned int maximum_required_images = 0;
    unsigned int image_count;

    /** @brief A helper function to simplify the code in the
     *  constructor. It requires reading in the template file
     *  to define the required value. Afterwards it closes
     *  the image.
     *
     *  @param template_image: A pointer to the template Image
     *  object in which the check is requested.
     *
     *  @throws InvalidTemplateException if the template's reading
     *  process fails
     */
    unsigned int check_number_of_required_images(Image* template_image);
public:
    /** @brief Constructor, creates a new TemplateContainer object.
     *  Requires a directory of templates.
     *
     *  @param directory: Directory path where the templates are located
     *  @param image_count: the number of input images that will be used
     *  during generation (the number of images loaded by InputDataContainer)
     *
     *  @throws Utils::Directory::PathNotFoundException if the given directory
     *  path is invalid
     */
    TemplateContainer(const std::string& directory, const unsigned int& image_count);

    /** @brief Destructs the allocated data in templates QVector
     *  by calling Image::close methods on the Image objects
     *  then deleting them.
     */
    ~TemplateContainer();

    /** @brief Selects a template to be use by Generation::generate.
     *  Selects a random number as an index of templates QVector.
     *
     *  @throws Utils::Exceptions::NoValidInputsLeftException if
     *  no templates are left. In this case the generation finishes.
    */
    Image* select_a_template();

    /** @brief To be called after an image is removed from InputDataContainer.
     *  Decreases the image counter. Removes all templates that requires more
     *  input image than what left.
     *
     *  @throws Utils::Exceptions::BadUsageException no more templates left
     */
    void on_image_deleted();
};

#endif // TEMPLATECONTAINER_H
